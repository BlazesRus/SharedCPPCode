// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "PartialMediumDec.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDec;

	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy to 9th digit
    /// Represents +- 2147483647.999999999 with 100% accuracy 
    /// except for truncation during division and multiplication after 9th digit
	/// </summary>
    class DLL_API MediumDec : protected PartialMediumDec
    {
public:
		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModResult<PartialMediumDec>{};

#pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDec& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDec& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const PartialMediumDec& rhs)
        {
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
        }

        MediumDec& operator=(const PartialMediumDec& rhs)
        {
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator)
        /// </summary>
        template<MediumDecVariant VariantType>
        auto operator()(VariantType variantValue) const
        {
            MediumDec newSelf = MediumDec(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

#pragma endregion class_constructors

        ///// <summary>
        ///// The decimal overflow value * -1
        ///// </summary>
		//static signed _int64 const NegDecimalOverflowX = -1000000000;

        void SetValue(MediumDec Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

        template<MediumDecVariant VariantType=MediumDec>
        void SetVariantValue(VariantType Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }


    #pragma region Const Representation values
    #pragma endregion Const Representation values
    
    #pragma region RepType
    #pragma endregion RepType

    #pragma region Comparison Operators

    #pragma region ValueDefines
    private:
	#if defined(AltNum_EnableNaN)
        static MediumDec NaNValue()
        {
            MediumDec NewSelf = MediumDec(0, NaNRep);
            return NewSelf;
        }
		
        static MediumDec UndefinedValue()
        {
            MediumDec NewSelf = MediumDec(0, UndefinedRep);
            return NewSelf;
        }
	#endif
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNumValue()
        {
            return MediumDec(3, 141592654);
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec HundredMilPiNumVal()
        {
            return MediumDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec TenMilPiNumVal()
        {
            return MediumDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec OneMilPiNumVal()
        {
            return MediumDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static MediumDec TenPiNumVal()
        {
            return MediumDec(31, 415926536);
        }
        
        static MediumDec ENumValue()
        {
            return MediumDec(2, 718281828);
        }
        
        static MediumDec ZeroValue()
        {
            return MediumDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneValue()
        {
            MediumDec NewSelf = MediumDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec TwoValue()
        {
            MediumDec NewSelf = MediumDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOneValue()
        {
            MediumDec NewSelf = MediumDec(MirroredInt(1,0));
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Point5Value()
        {
            MediumDec NewSelf = MediumDec(0, 500000000);
            return NewSelf;
        }

        static MediumDec JustAboveZeroValue()
        {
            MediumDec NewSelf = MediumDec(0, 1);
            return NewSelf;
        }

        static MediumDec OneMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 1000);
            return NewSelf;
        }

        static MediumDec FiveThousandthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5000000);
            return NewSelf;
        }

        static MediumDec FiveMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5000);
            return NewSelf;
        }

        static MediumDec TenMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 100);
            return NewSelf;
        }

        static MediumDec OneHundredMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 10);
            return NewSelf;
        }

        static MediumDec FiveBillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5);
            return NewSelf;
        }

        static MediumDec LN10Value()
        {
            return MediumDec(2, 302585093);
        }

        static MediumDec LN10MultValue()
        {
            return MediumDec(0, 434294482);
        }

        static MediumDec HalfLN10MultValue()
        {
            return MediumDec(0, 868588964);
        }
        
    #if defined(AltNum_EnableNil)
        static MediumDec NilValue()
        {
            return MediumDec(NilRep, NilRep);
        }
    #endif

        static MediumDec MinimumValue()
        {
            return MediumDec(MinIntHalf, 999999999);
        }

        static MediumDec MaximumValue()
        {
            return MediumDec(MaxIntHalf, 999999999);
        }

public:
        static MediumDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec ENum;
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneGMillionth;

        //0e-7
        static MediumDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDec HalfLN10Mult;

    #if defined(AltNum_EnableNil)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDec Nil;
    #endif
public:
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value);

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        auto GetValueFromString(std::string Value)
        {

        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDec(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const std::string& Value)
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
		//To-Do: Add more exact conversion from floating point format to MediumDec variant
	
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetFloatVal(const float& Value)
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
        void SetDecimalVal(const ldouble& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			ldouble lValue = Value;
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
                lValue -= (ldouble)WholeValue;
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
        virtual void SetBoolVal(const bool& Value)
        {
            IntHalf = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntVal(const int& Value)
        {
			if(Value<0)
			{
				IntHalf.Sign = 0;
				IntHalf.Value = -Value;
			}
			else
				IntHalf = Value;
			DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecdSetValues)
        MediumDec(const MediumDec& Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
protected://Adding more exact conversion from floating point to MediumDec variant later

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void float toFloatV1()
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
        void double toDoubleV1()
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
        void ldouble toDecimalV1()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            ldouble Value;
            if (IntHalf < 0)
            {
                Value = (ldouble)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (ldouble)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
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

public:
        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat()
        {
            return toFloatV1;
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble()
        {
            return toDoubleV1();
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDecimal()
        {
            return toDecimalV1;
        }
		
        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() { return IntHalf; }

        bool toBool() { return IntHalf.IsZero() ? false : true; }

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
	
    #pragma region Other RepType Conversion
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDec>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
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

		std::strong_ordering operator<=>(const MediumDec& that) const
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

		bool operator==(const int& that) const
		{
			if (IntHalf!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator==(const MediumDec& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.IntHalf)
				return false;
		}

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntDivOperationV1(const IntType& Value)
        {
            BasicUIntDivOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntDivOperationV1(const IntType& Value)
        {
            BasicIntDivOpV1(Value); return *this;
        }

        MediumDec& BasicUIntDivOperation() { BasicUIntDivOperationV1<unsigned int>; }
        MediumDec& BasicIntDivOperation() { BasicIntDivOperationV1<signed int>; }
        MediumDec& BasicUInt64DivOperation() { BasicUIntDivOperationV1<unsigned long long>; }
        MediumDec& BasicInt64DivOperation() { BasicIntDivOperationV1<signed long long>; }
        MediumDec& BasicUInt8DivOperation() { BasicUIntDivOperationV1<unsigned char>; }
        MediumDec& BasicInt8DivOperation() { BasicIntDivOperationV1<signed char>; }
        MediumDec& BasicUInt16DivOperation() { BasicUIntDivOperationV1<unsigned short>; }
        MediumDec& BasicInt16DivOperation() { BasicIntDivOperationV1<signed short>; }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const MediumDec& rValue) { return self.DivideBy(rValue); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const MediumDec& rValue) { return self.DivOperation(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const signed int& rValue) { return self.BasicDivideByInt(rValue); }
        friend MediumDec operator/(const MediumDec& self, const signed long long& rValue) { return self.BasicDivideByInt64(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned int& rValue) { return self.BasicDivideByUInt(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned long long& rValue) { return self.BasicDivideByUInt64(rValue); }
		
        friend MediumDec operator/(const signed int& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed long long& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned long long& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }

        friend MediumDec operator/(const MediumDec& self, const signed char& rValue) { return self.BasicDivideByInt8(rValue); }
        friend MediumDec operator/(const MediumDec& self, const signed short& rValue) { return self.BasicDivideByInt16(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned char& rValue) { return self.BasicDivideByUInt8(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned short& rValue) { return self.BasicDivideByUInt16(rValue); }

        friend MediumDec operator/(const signed char& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed short& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned char& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned short& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const signed int& rValue) { return self.BasicIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const Int64& rValue) { return self.BasicInt64DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned int& rValue) { return self.BasicUIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const UInt64& rValue) { return self.BasicUInt64DivOperation(rValue); }

        friend MediumDec& operator/=(MediumDec& self, const signed char& rValue) { return self.BasicInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const signed short& rValue) { return self.BasicInt16DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned char& rValue) { return self.BasicUInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned short& rValue) { return self.BasicUInt16DivOperation(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntMultOperationV1(const IntType& Value)
        {
            BasicUIntMultOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntMultOperationV1(const IntType& Value)
        {
            BasicIntMultOpV1(Value); return *this;
        }

	    MediumDec& BasicUIntMultOperation() { BasicUIntMultOperationV1<unsigned int>; }
        MediumDec& BasicIntMultOperation() { BasicIntMultOperationV1<signed int>; }
        MediumDec& BasicUInt64MultOperation() { BasicUIntMultOperationV1<unsigned long long>; }
        MediumDec& BasicInt64MultOperation() { BasicIntMultOperationV1<signed long long>; }
        MediumDec& BasicUInt8MultOperation() { BasicUIntMultOperationV1<unsigned char>; }
        MediumDec& BasicInt8MultOperation() { BasicIntMultOperationV1<signed char>; }
        MediumDec& BasicUInt16MultOperation() { BasicUIntMultOperationV1<unsigned short>; }
        MediumDec& BasicInt16MultOperation() { BasicIntMultOperationV1<signed short>; }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& self, const MediumDec& rValue) { return self.MultipleBy(rValue); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& self, const MediumDec& rValue) { return self.MultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& lValue, const signed int& rValue) { return lValue.BasicMultiplyByInt(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicMultiplyByInt64(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicMultiplyByUInt(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicMultiplyByUInt64(rValue); }
		
        friend MediumDec operator*(const signed int& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt(lrValue); }
        friend MediumDec operator*(const unsigned long long& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt64(lrValue); }
        friend MediumDec operator*(const unsigned int& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt(lrValue); }
        friend MediumDec operator*(const unsigned long long& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt64(lrValue); }

        friend MediumDec operator*(const MediumDec& lValue, const signed char& rValue) { return lValue.BasicMultiplyByInt8(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const signed short& rValue) { return lValue.BasicMultiplyByInt16(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicMultiplyByUInt8(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicMultiplyByUInt16(rValue); }

        friend MediumDec operator*(const signed char& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt8(lrValue); }
        friend MediumDec operator*(const signed short& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt16(lrValue); }
        friend MediumDec operator*(const unsigned char& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt8(lrValue); }
        friend MediumDec operator*(const unsigned short& lrValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt16(lrValue); }

        /// <summary>
        /// *= operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed long long& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64MultOperation(rValue); }

        friend MediumDec& operator*=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }

	#pragma endregion Other Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOperationV1(const IntType& Value)
        {
            BasicUIntAddOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntAddOperationV1(const IntType& Value)
        {
            BasicIntAddOpV1(Value); return *this;
        }

        MediumDec& BasicUIntAddOperation() { BasicUIntAddOperationV1<unsigned int>; }
        MediumDec& BasicIntAddOperation() { BasicIntAddOperationV1<signed int>; }
        MediumDec& BasicUInt64AddOperation() { BasicUIntAddOperationV1<unsigned long long>; }
        MediumDec& BasicInt64AddOperation() { BasicIntAddOperationV1<signed long long>; }
        MediumDec& BasicUInt8AddOperation() { BasicUIntAddOperationV1<unsigned char>; }
        MediumDec& BasicInt8AddOperation() { BasicIntAddOperationV1<signed char>; }
        MediumDec& BasicUInt16AddOperation() { BasicUIntAddOperationV1<unsigned short>; }
        MediumDec& BasicInt16AddOperation() { BasicIntAddOperationV1<signed short>; }

    #pragma endregion NormalRep Integer Addition Operations

	#pragma region Other Addition operations

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& self, const MediumDec& rValue) { return self.AdditionBy(rValue); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& self, const MediumDec& rValue) { return self.AdditionOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& lValue, const signed int& rValue) { return lValue.BasicAddByInt(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicAddByInt64(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicAddByUInt(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicAddByUInt64(rValue); }
		
        friend MediumDec operator+(const signed int& lrValue, const MediumDec& rValue) { return rValue.BasicAddByInt(lrValue); }
        friend MediumDec operator+(const unsigned long long& lrValue, const MediumDec& rValue) { return rValue.BasicAddByInt64(lrValue); }
        friend MediumDec operator+(const unsigned int& lrValue, const MediumDec& rValue) { return rValue.BasicAddByUInt(lrValue); }
        friend MediumDec operator+(const unsigned long long& lrValue, const MediumDec& rValue) { return rValue.BasicAddByUInt64(lrValue); }

        friend MediumDec operator+(const MediumDec& lValue, const signed char& rValue) { return lValue.BasicAddByInt8(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const signed short& rValue) { return lValue.BasicAddByInt16(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicAddByUInt8(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicAddByUInt16(rValue); }

        friend MediumDec operator+(const signed char& lrValue, const MediumDec& rValue) { return rValue.BasicAddByInt8(lrValue); }
        friend MediumDec operator+(const signed short& lrValue, const MediumDec& rValue) { return rValue.BasicAddByInt16(lrValue); }
        friend MediumDec operator+(const unsigned char& lrValue, const MediumDec& rValue) { return rValue.BasicAddByUInt8(lrValue); }
        friend MediumDec operator+(const unsigned short& lrValue, const MediumDec& rValue) { return rValue.BasicAddByUInt16(lrValue); }

        /// <summary>
        /// += operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed long long& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64AddOperation(rValue); }

        friend MediumDec& operator+=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }

	#pragma endregion Other Addition operations

	#pragma region NormalRep Integer Subtraction Operations

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntSubOperationV1(const IntType& Value)
        {
            BasicUIntSubOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntSubOperationV1(const IntType& Value)
        {
            BasicIntSubOpV1(Value); return *this;
        }

        MediumDec& BasicUIntSubOperation() { BasicUIntSubOperationV1<unsigned int>; }
        MediumDec& BasicIntSubOperation() { BasicIntSubOperationV1<signed int>; }
        MediumDec& BasicUInt64SubOperation() { BasicUIntSubOperationV1<unsigned long long>; }
        MediumDec& BasicInt64SubOperation() { BasicIntSubOperationV1<signed long long>; }
        MediumDec& BasicUInt8SubOperation() { BasicUIntSubOperationV1<unsigned char>; }
        MediumDec& BasicInt8SubOperation() { BasicIntSubOperationV1<signed char>; }
        MediumDec& BasicUInt16SubOperation() { BasicUIntSubOperationV1<unsigned short>; }
        MediumDec& BasicInt16SubOperation() { BasicIntSubOperationV1<signed short>; }

    #pragma endregion NormalRep Integer Subtraction Operations

	#pragma region Other Subtraction Operations

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& self, const MediumDec& rValue) { return self.SubtractBy(rValue); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, const MediumDec& rValue) { return self.SubtractionOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& self, const signed int& rValue) { return self.BasicSubtractByInt(rValue); }
        friend MediumDec operator-(const MediumDec& self, const signed long long& rValue) { return self.BasicSubtractByInt64(rValue); }
        friend MediumDec operator-(const MediumDec& self, const unsigned int& rValue) { return self.BasicSubtractByUInt(rValue); }
        friend MediumDec operator-(const MediumDec& self, const unsigned long long& rValue) { return self.BasicSubtractByUInt64(rValue); }
		
        friend MediumDec operator-(const signed int& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed long long& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned long long& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }

        friend MediumDec operator-(const MediumDec& self, const signed char& rValue) { return self.BasicSubtractByInt8(rValue); }
        friend MediumDec operator-(const MediumDec& self, const signed short& rValue) { return self.BasicSubtractByInt16(rValue); }
        friend MediumDec operator-(const MediumDec& self, const unsigned char& rValue) { return self.BasicSubtractByUInt8(rValue); }
        friend MediumDec operator-(const MediumDec& self, const unsigned short& rValue) { return self.BasicSubtractByUInt16(rValue); }

        friend MediumDec operator-(const signed char& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lrValue, const MediumDec& rValue) { return ((MediumDec)lrValue).SubtractBy(rValue); }


        /// <summary>
        /// += operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side rValue</param>
        /// <param name="rValue">The right side rValue.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, const signed int& rValue) { return self.BasicIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const Int64& rValue) { return self.BasicInt64SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned int& rValue) { return self.BasicUIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const UInt64& rValue) { return self.BasicUInt64SubOperation(rValue); }

        friend MediumDec& operator-=(MediumDec& self, const signed char& rValue) { return self.BasicInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const signed short& rValue) { return self.BasicInt16SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned char& rValue) { return self.BasicUInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned short& rValue) { return self.BasicUInt16SubOperation(rValue); }

	#pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
    //Defined inside full version of class object
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

    #pragma region Floating Operator Overrides
    
        friend MediumDec operator+(const MediumDec& self, const float& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const float& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const float& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const float& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const float& Value, const MediumDec& self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(const float& Value, const MediumDec& self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(const float& Value, const MediumDec& self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(const float& Value, const MediumDec& self) { return (MediumDec)Value / self; }

        friend MediumDec operator+(const MediumDec& self, const double& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const double& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const double& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const double& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const MediumDec& self, const ldouble& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const ldouble& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const ldouble& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const ldouble& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const ldouble& Value, const MediumDec& self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(const ldouble& Value, const MediumDec& self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(const ldouble& Value, const MediumDec& self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(const ldouble& Value, const MediumDec& self) { return (MediumDec)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDec</returns>
        MediumDec operator -()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator ++()
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
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator --()
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
        /// <returns>MediumDec</returns>
        MediumDec operator ++(int)
        {
            MediumDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator --(int)
        {
            MediumDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

// Static versions of functions for Full versions
	#pragma region Math Etc Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDec&</returns>
        auto& AbsOf()
        {
            if (IntHalf.IsNegative())
                IntHalf.Sign = 1;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static auto Abs(const auto tValue&)
        {
            auto self = tValue;
            return self.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        auto& FloorOf()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static auto Floor(const auto tValue&, const int precision&)
        {
            auto self = tValue;
            switch (precision)
            {
            case 9: break;
            case 8: self.DecimalHalf /= 10; Value.DecimalHalf *= 10; break;
            case 7: self.DecimalHalf /= 100; Value.DecimalHalf *= 100; break;
            case 6: self.DecimalHalf /= 1000; Value.DecimalHalf *= 1000; break;
            case 5: self.DecimalHalf /= 10000; Value.DecimalHalf *= 10000; break;
            case 4: self.DecimalHalf /= 100000; Value.DecimalHalf *= 100000; break;
            case 3: self.DecimalHalf /= 1000000; Value.DecimalHalf *= 1000000; break;
            case 2: self.DecimalHalf /= 10000000; Value.DecimalHalf *= 10000000; break;
            case 1: self.DecimalHalf /= 100000000; Value.DecimalHalf *= 100000000; break;
            default: self.DecimalHalf = 0; break;
            }
            if (self.IntHalf == NegativeRep && Value.DecimalHalf == 0) { self.IntHalf = 0; }
            return self;
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        auto& CeilOf()
        {
            if (DecimalHalf != 0)
            {
                DecimalHalf = 0;
                if (IntHalf == NegativeRep) { IntHalf = 0; }
                else
                {
                    ++IntHalf;
                }
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        static int FloorInt(const auto& tValue)
        {
            if (tValue.DecimalHalf == 0)
            {
                return tValue.IntHalf.Value;
            }
            if (tValue.IntHalf == NegativeRep) { return -1; }
            else
            {
                return tValue.IntHalf.Value - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        static int CeilInt(const auto& tValue)
        {
            if (tValue.DecimalHalf == 0)
            {
                return self.IntHalf.Value;
            }
            if (tValue.IntHalf == NegativeRep) { return 0; }
            else
            {
                return tValue.IntHalf.Value + 1;
            }
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static auto Ceil(const auto& tValue)
        {
            auto self = tValue;
            return self.Ceil();
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDec &</returns>
        auto& Trunc()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static auto Trunc(const auto& Value)
        {
            auto self = tValue;
            return self.Trunc();
        }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        auto SqrtOf(const int& precision=7)
        { 
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            else if (DecimalHalf == 0)
            {
                auto value = *this;
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

            auto number = this;
            auto start = Zero, end = number;
            auto mid;

            // variable to store the answer 
            auto ans;

            // for computing integral part 
            // of square root of number 
            while (start <= end) {
                mid = (start + end) / 2;
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
            auto increment = "0.1";
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
		
		/// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
		static auto Sqrt(const auto& value, const int& precision=7)
		{
			return value.SqrtOf(precision);
		}

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOpV1(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfOpV1(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                ValueType exp = expValue * -1;
                if (DecimalHalf == 0 && IntHalf == 10 && expValue >= -9)
                {
                    IntHalf = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&exp&1==1)
                        Sign = 1;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                    auto self = AbsOf();//Prevent needing to flip the sign
                    IntHalf = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (exp & 1 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self * self; //  Change x to x^2
                    }
                    if(IsNegative)
                        IntHalf.Sign = 0;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                //Switches from negative to positive if exp is odd number
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue & 1 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = 0;
            }
            return *this;
        }
		
        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.UIntPowOpV1();
		}
		
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.IntPowOpV1();
		}

public:

		auto UnsignedNegIntPower(const unsigned int& exp)
		{
			ResetDivisor();
			//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
			//Switches from negative to positive if exp is odd number
			bool IsNegative = IsPositive()?false:exp&1==1?false:true;
			auto self = AbsOf();
			IntHalf = 1; DecimalHalf = 0;// Initialize result
			while (expValue > 0)
			{
				// If expValue is odd, divide self with result
				if (exp & 1 == 1)
					*this /= self;
				// n must be even now
				expValue = expValue >> 1; // y = y/2
				self *= self; // Change x to x^2
			}
			if(IsNegative)
				IntHalf.Sign = 0;
		}

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOfOp = UIntPowOfOpV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOfOp = IntPowOfOpV1<signed int>;
        constexpr auto UInt64PowOfOp = UIntPowOfOpV1<UInt64>;
        constexpr auto Int64PowOfOp = IntPowOpOfV1<Int64>;
        
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOf = UIntPowOfV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOf = IntPowOfV1<signed int>;
        constexpr auto UInt64PowOf = UIntPowOfV1<UInt64>;
        constexpr auto Int64PowOf = IntPowOfV1<Int64>;

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        auto NthRootOf(const int& n, const auto& precision = auto::JustAboveZero)
        {
            auto xPre = ((this - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            auto delX = auto(2147483647, 0);

            //  xK denotes current value of x 
            auto xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (xPre * nMinus1 + this / auto::Pow(xPre, nMinus1)) / n;
                delX = auto::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        static auto FractionalPow(const auto& value, const int& expNum, const int& expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static auto FractionalPow(const auto& value, const boost::rational<int>& Frac);

        void BasicPowOp(const auto& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        auto PowOp(const auto& expValue);

        static auto PowOp(const auto& targetValue, auto& expValue)
        {
            return targetValue.PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static auto Pow(const auto& targetValue, auto expValue)
        {
            return PowOp(targetValue, expValue);
        }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <returns>MediumDec</returns>
        auto ExpOf()
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
             *      - approximation of e^x in MediumDec precision
             */
             // Check that x is a valid input.
            assert(IntHalf.Value < 709);

            // When x = 0 we already know e^x = 1.
            if (IsZero()) {
                return One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            auto x0 = *this;
            x0.Abs();
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            auto tn = One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = CeilInt(x0 * E) * 12;
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
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>BlazesRusCode::MediumDec</returns>
        static auto Exp(const auto& x)
        {
			return x.ExpOf();
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        auto NthRootOf(const int& n, const auto& Precision = FiveBillionth)
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

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static auto NthRoot(const auto& value, const int& n, const auto& Precision = FiveBillionth)
        {
            return value.NthRootOf(n, Precision);
        }

protected:

		auto LnRef_Part02()
		{	
            //Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
			//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
			auto TotalRes = (this - 1) / (this + 1);
			auto LastPow = TotalRes;
			auto WSquared = TotalRes * TotalRes;
			auto AddRes;
			int WPow = 3;
			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes += AddRes; WPow += 2;
			} while (AddRes > JustAboveZero);
			return TotalRes;
		}

public:

		/// <summary>
		/// Natural log (Equivalent to Log_E(value))
		/// </summary>
		/// <returns>BlazesRusCode::MediumDec</returns>
		auto NaturalLogOf()
		{
			if(IntHalf.IsNegative)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from natural log";
			if (IsOne())
				return Zero;
            if(IntHalf==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                auto W = (value - 1)/ (value + 1);
                auto TotalRes = -W;
                auto LastPow = W;
                auto WSquared = W * W;
                int WPow = 3;
                auto AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDec::JustAboveZero);
                return TotalRes * 2;
            }
            else if (IntHalf==1)//Threshold between 1 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {
                //This section gives accurate answer(for values between 1 and 2)
                auto threshold = MediumDec::FiveMillionth;
                auto base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev;          // Previous sum
                auto result = term;     // Kick it off

                do
                {
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                    ++den;
                } while (MediumDec::Abs(prev - result) > threshold);

                return result;
            }
			else
                //Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
                //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
				return LnRef_Part02() * 2;
		}
	
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>BlazesRusCode::MediumDec</returns>
        static auto Ln(const auto& value)
        {
			return value.NaturalLogOf();
        }
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
		auto Log10Of()
		{
			if(IntHalf.IsNegative)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log base 10";
			if (IsOne())
				return Zero;
            if (DecimalHalf == 0 && IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
            }
            if (IntHalf<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                auto threshold = FiveBillionth;
                auto base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev = 0;          // Previous sum
                auto result = term;     // Kick it off

                while (Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*LN10Mult;// result/MediumDec::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return LnRef_Part02() * HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
            }
		}
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
        static auto Log10(const auto& value)
        {
			return value.Log10Of();
        }
		
protected:

    template<IntegerType IntType=unsigned int>
    static auto Log10_IntPart02(const IntType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        auto TotalRes = MediumDec(value - 1) / MediumDec(value + 1);
        auto LastPow = TotalRes;
        auto WSquared = TotalRes * TotalRes;
        auto AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDec::JustAboveZero);
        return TotalRes * MediumDec::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
	
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
		template<IntegerType IntType=unsigned int>
        static auto Log10OfInt(const IntType& value)
        {
			if(value<0)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log base 10";
            else if (value == 1)
                return MediumDec::Zero;
            else if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_IntPart02(value);
            }
        }
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        auto LogOf(const auto& baseVal)
        {
            if (IsOne())
                return Zero;
            return Log10Of() / baseVal.Log10Of();
        }
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        static auto Log(const auto& value, const auto& baseVal)
        {
            return value.LogOf(baseVal);
        }

protected:

        bool LogOfInt_BaseCalculation(const int& baseVal, auto& baseTotalRes)
        {
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDec(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDec((baseVal - 1), 0) / MediumDec((baseVal + 1), 0);
                auto baseLastPow = baseTotalRes;
                auto baseWSquared = baseTotalRes * baseTotalRes;
                auto baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > JustAboveZero);
            }
            return true;
        }

        bool LogOf_BaseCalculation(const auto& baseVal, auto& baseTotalRes)
        {
            if (baseVal.DecimalHalf==0&&baseVal.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDec(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDec((baseVal - 1), 0) / MediumDec((baseVal + 1), 0);
                auto baseLastPow = baseTotalRes;
                auto baseWSquared = baseTotalRes * baseTotalRes;
                auto baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > JustAboveZero);
            }
            return true;
        }

        auto LogOf_Section02(const bool& lnMultLog, const auto& baseTotalRes, const auto& threshold)
        {
            //Now calculate other log
			if(IsNegative())//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log";
            if (DecimalHalf == 0 && IntHalf % 10 == 0)
            {
                if(lnMultLog)
                {
                    for (int index = 1; index < 9; ++index)
                    {
                        if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                            return MediumDec(index, 0) / (baseTotalRes * HalfLN10Mult);
                    }
                    return MediumDec(9, 0) / (baseTotalRes*HalfLN10Mult);
                }
                else
                {
                    for (int index = 1; index < 9; ++index)
                    {
                        if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                            return MediumDec(index, 0)/ baseTotalRes;
                    }
                    return MediumDec(9, 0)/baseTotalRes;
                }
            }
            if (IntHalf.Value<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                auto base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev = Zero;          // Previous sum
                auto result = term;     // Kick it off

                while (Abs(prev - result) > threshold) {
                    ++den;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                if(lnMultLog)
                    return result/baseTotalRes;
                else
                    return (result*2)/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                auto W = (this - 1) / (this + 1);
                auto TotalRes = W;
                auto AddRes;
                int WPow = 3;
                do
                {
                    AddRes = Pow(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > JustAboveZero);
                if(lnMultLog)
                    return TotalRes/baseTotalRes;
                else
                    return (TotalRes * HalfLN10Mult)/ baseTotalRes;
            }
        }

public:

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        auto LogOfInt(const int& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOfInt_BaseCalculation(baseTotalRes);
            return LogOf_Section02(lnMultLog, baseTotalRes, threshold);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        auto LogOfV2(const auto& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOf_BaseCalculation(baseTotalRes);
            return LogOf_Section02(lnMultLog, baseTotalRes, threshold);
        }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static auto Sin(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(Value, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static auto Cos(const auto& Value)
        {
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static auto Tan(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
        static auto ATan(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
            for (int i = 1; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue / SinValue;
        }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDec</returns>
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
        /// <returns>MediumDec</returns>
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
                        auto Radius = Pi * Value / 180;
                        return Sin(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                auto Radius = Pi * Value / 180;
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
                auto Radius = Pi * Value / 180;
                return Cos(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
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
                        return Tan(Pi * Value / 180);
                        break;
                }
            }
            else
                return Tan(Pi * Value / 180);
        }

    #pragma endregion Trigonomic Functions
    };

    #pragma region ValueDefine Source

	MirroredInt MediumDec::NegativeRep = MirroredInt::NegativeZero;
	MirroredInt MediumDec::MaxIntHalf = MirroredInt::Maximum;
	MirroredInt MediumDec::MinIntHalf = MirroredInt::Minimum;

    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDec::ReadString(const std::string& Value)
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
        std::string Value = std::string(IntHalf);
        if (DecimalHalf != 0)
        {
            Value += ".";
            Value += std::string(DecimalHalf)
        }
        return Value;
    }

    std::string MediumDec::ToFullString()
    {
        std::string Value = "";
        unsigned int CurrentSection = IntHalf.Value;
        unsigned __int8 CurrentDigit;
        if (IsNegative())
            Value += "-";
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