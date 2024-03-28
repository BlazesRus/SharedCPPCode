// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#ifdef BlazesSharedCode_LocalLayout
#ifndef DLL_API
#ifdef UsingBlazesSharedCodeDLL
#define DLL_API __declspec(dllimport)
#elif defined(BLAZESSharedCode_LIBRARY)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif
#endif
#else
#include "..\DLLAPI.h"
#endif

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#if defined(AltNum_UseOldDivisionCode)
	#include <boost/multiprecision/cpp_int.hpp>
#endif

#include "AltNumModChecker.hpp"

#include "..\MediumDec\MediumDecBase.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/

namespace BlazesRusCode
{

    class MediumDec;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// (8 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API MediumDec : public virtual MediumDecBase
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const int& intVal, const signed int& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        MediumDec(const MediumDecBase&) = default;

        MediumDec& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

    #pragma region Const Representation values
    //Variables for this section stored inside derivable base class
    #pragma endregion Const Representation values

    #pragma region RepType
		
		static std::string RepTypeAsString(RepType& repType)
		{
			switch(repType)
			{
				case RepType::NormalType:
					return "NormalType"; break;
	#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:
					return "NumByDiv"; break;
	#endif
	#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
					return "PiNum"; break;
		#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower:
					return "PiPower"; break;
		#endif
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
					return "PiNumByDiv"; break;
			#else
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					return "PiFractional"; break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
					return "ENum"; break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
					return "ENumByDiv"; break;
			#else
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					return "EFractional"; break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
                    return "INum"; break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
					return "INumByDiv"; break;
			#else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					return "IFractional"; break;
			#endif
		#endif
		#ifdef AltNum_EnableComplexNumbers
				case RepType::ComplexIRep:
					return "ComplexIRep"; break;
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
					return "MixedFrac"; break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedPi"; break;
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedE"; break;
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedI"; break;
		#endif
	#endif

	#if defined(AltNum_EnableInfinityRep)
				case RepType::PositiveInfinity://If Positive Infinity: then convert number into MaximumValue instead when need as real number
					return "PositiveInfinity"; break;
				case RepType::NegativeInfinity://If Negative Infinity: then convert number into MinimumValue instead when need as real number
					return "NegativeInfinity"; break;
	#endif
	#if defined(AltNum_EnableApproachingValues)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    return "ApproachingBottom"; break;
		#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    return "ApproachingTop"; break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:
					return "ApproachingMidLeft"; break;
				case RepType::ApproachingMidRight:
					return "ApproachingMidRight"; break;
		#endif
	#endif
    #if defined(AltNum_EnableNaN)
				case RepType::Undefined:
					return "Undefined"; break;
				case RepType::NaN:
					return "NaN"; break;
    #endif
	#if defined(AltNum_EnableApproachingPi)
				case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
					return "ApproachingTopPi"; break;
	#endif
	#if defined(AltNum_EnableApproachingE)
				case RepType::ApproachingTopE://equal to IntValue.9..9 e
					return "ApproachingTopE"; break;
	#endif
	#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::PositiveImaginaryInfinity:
					return "PositiveImaginaryInfinity"; break;
				case RepType::NegativeImaginaryInfinity:
					return "NegativeImaginaryInfinity"; break;
	#endif
	#if defined(AltNum_EnableApproachingI)
				case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					return "ApproachingImaginaryBottom"; break;
		#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					return "ApproachingImaginaryTop"; break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:
					return "ApproachingImaginaryMidLeft"; break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidRight:
					return "ApproachingImaginaryMidRight"; break;
			#endif
		#endif
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range: ExtraRepValue==UndefinedInRangeRep)
				case RepType::UndefinedButInRange:
					return "UndefinedButInRange"; break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
				case RepType::WithinMinMaxRange:
					return "WithinMinMaxRange"; break;
		#endif
	#endif
    #if defined(AltNum_EnableNilRep)
				case RepType::Nil:
					return "Nil"; break;
    #endif
				default:
					return "Unknown";
			}
		}

    #pragma endregion RepType

public:
	#pragma region PiNum Setters
	//Not used for this variant
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
	//Not used for this variant
	#pragma endregion ENum Setters

	#pragma region Fractional Setters
	//Not used for this variant
	#pragma endregion Fractional Setters
        
	#pragma region MixedFrac Setters
	//Not used for this variant
	#pragma endregion MixedFrac Setters
		
	#pragma region Infinity Setters
	//Stored in Base Class
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Stored in Base Class
	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
	//Stored inside Base Class
	#pragma endregion NaN Setters

    #pragma region ValueDefines
    protected:
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
            MediumDec NewSelf = MediumDec(-1);
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
        
    #if defined(AltNum_EnableNilRep)
        static MediumDec NilValue()
        {
            return MediumDec(NilRep, NilRep);
        }
    #endif

        static MediumDec MinimumValue()
        {
            return MediumDec(2147483647, 999999999);
        }

        static MediumDec MaximumValue()
        {
            return MediumDec(2147483647, 999999999);
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
        
#if defined(AltNum_EnableInfinityRep)
        static MediumDec Infinity;
        static MediumDec NegativeInfinity;
        static MediumDec ApproachingZero;
#endif
        
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

    #if defined(AltNum_EnableNilRep)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDec Nil;
    #endif
public:
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDec(const char* strVal)
        {
            ReadFromCharString(strVal);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(std::string Value)
        {
            ReadFromString(Value);
        }
    #pragma endregion String Commands

public:
    #pragma region ConvertFromOtherTypes

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(float Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(double Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(ldouble Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(bool Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecBasedSetValues)
        MediumDec(MediumDec Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        /// <summary>
        /// MediumDec to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }

        /// <summary>
        /// MediumDec to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double(){ return toDouble(); }

        /// <summary>
        /// MediumDec to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble() { return toDecimal(); }
		
        /// <summary>
        /// MediumDec to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return toInt(); }

        explicit operator bool() { return toBool(); }
    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
    std::strong_ordering operator<=>(const MediumDec& that) const
    {
        int lVal = IntValue==NegativeZero?0:IntValue;
        int rVal = that.IntValue==NegativeZero?0:that.IntValue;
        if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
            return IntHalfCmp;
        //Counting negative zero as same as zero IntValue but with negative DecimalHalf
        lVal = IntValue==NegativeZero?0-DecimalHalf:DecimalHalf;
        rVal = IntValue==NegativeZero?0-that.DecimalHalf:that.DecimalHalf;
        if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
    }

    std::strong_ordering operator<=>(const int& that) const
    {
        int lVal = IntValue==NegativeZero?0:IntValue;
        int rVal = that;
        if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
            return IntHalfCmp;
        //Counting negative zero as same as zero IntValue but with negative DecimalHalf
        lVal = DecimalHalf>0?1:0;
        if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
    }

    bool operator==(const int& that) const
    {
        if (IntValue!=that)
            return false;
        if (DecimalHalf!=0)
            return false;
        return true;
    }

    bool operator==(const MediumDec& that) const
    {
        if (IntValue!=that.IntValue)
            return false;
        if (DecimalHalf!=that.IntValue)
            return false;
    }
    #pragma endregion Comparison Operators

	protected:
	#pragma region Division Operations

        /// <summary>
        /// Division Operation
        /// </summary>
        MediumDec& DivOp(MediumDec& Value) { BasicDivOp(Value); return *this; }

        MediumDec DivideAsCopy(MediumDec Value) { MediumDec self = *this; self.BasicDivOp(Value); return self; }

	#pragma endregion Division Operations

	#pragma region Multiplication Operations

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        MediumDec& MultOp(MediumDec& Value) { BasicMultOp(Value); return *this; }

        MediumDec MultipleAsCopy(MediumDec Value) { MediumDec self = *this; self.BasicMultOp(Value); return self; }

	#pragma endregion Multiplication Operations

	#pragma region Addition Operations

        /// <summary>
        /// Addition Operation
        /// </summary>
        MediumDec& AddOp(MediumDec& Value) { BasicAddOp(Value); return *this; }

        MediumDec AddAsCopy(MediumDec Value) { MediumDec self = *this; self.BasicAddOp(Value); return self; }

	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        MediumDec& SubOp(MediumDec& Value) { BasicSubOp(Value); return *this; }

        MediumDec SubtractAsCopy(MediumDec Value) { MediumDec self = *this; self.BasicSubOp(Value); return self; }

	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations

        //MediumDec& RemOp(MediumDec& Value) { BasicRemOp(Value); return *this; }

        //MediumDec ModulusAsCopy(MediumDec Value) { MediumDec self = *this; self.BasicRemOp(Value); return self; }

	#pragma endregion Modulus Operations
	
	public:
    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec self, MediumDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, MediumDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(MediumDec self, MediumDec Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& self, MediumDec Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec self, MediumDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& self, MediumDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec self, MediumDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, MediumDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between MediumDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        friend MediumDec& operator+=(MediumDec& self, int Value) { return IntAddOp(self, Value); }

        //friend MediumDec operator+=(MediumDec* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, int Value) { return IntSubOp(self, Value); }

        //friend MediumDec& operator-=(MediumDec* self, int Value) { return IntSubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(MediumDec self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        friend MediumDec& operator*=(MediumDec& self, int Value) { return IntMultOp(self, Value); }

        ///// <summary>
        ///// *= Operation Between MediumDec and Integer Value (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        //friend MediumDec operator*=(MediumDec* self, int Value) { return IntMultOp(**self, Value); }

        /// <summary>
        /// Division Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, int Value) { return IntDivOp(self, Value); }

        //friend MediumDec operator/=(MediumDec* self, int Value) { return IntDivOp(**self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between MediumDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        friend MediumDec& operator+=(MediumDec& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(MediumDec self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        friend MediumDec operator*=(MediumDec& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/=(MediumDec& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend MediumDec operator+(MediumDec self, float Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(MediumDec self, float Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(MediumDec self, float Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(MediumDec self, float Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(float Value, MediumDec self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(float Value, MediumDec self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(float Value, MediumDec self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(float Value, MediumDec self) { return (MediumDec)Value / self; }

        friend MediumDec operator+(MediumDec self, double Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(MediumDec self, double Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(MediumDec self, double Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(MediumDec self, double Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(MediumDec self, ldouble Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(MediumDec self, ldouble Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(MediumDec self, ldouble Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(MediumDec self, ldouble Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(ldouble Value, MediumDec self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(ldouble Value, MediumDec self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(ldouble Value, MediumDec self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(ldouble Value, MediumDec self) { return (MediumDec)Value / self; }


        friend MediumDec operator+(MediumDec self, unsigned char Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-(MediumDec self, unsigned char Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*(MediumDec self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/(MediumDec self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend MediumDec operator+=(MediumDec& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-=(MediumDec& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*=(MediumDec& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/=(MediumDec& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend MediumDec operator+(MediumDec self, unsigned short Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-(MediumDec self, unsigned short Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*(MediumDec self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/(MediumDec self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend MediumDec operator+=(MediumDec& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-=(MediumDec& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*=(MediumDec& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/=(MediumDec& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend MediumDec operator+(MediumDec self, unsigned int Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-(MediumDec self, unsigned int Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*(MediumDec self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/(MediumDec self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        

        friend MediumDec operator+=(MediumDec& self, unsigned int Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-=(MediumDec& self, unsigned int Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*=(MediumDec& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/=(MediumDec& self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        
        friend MediumDec operator+(MediumDec self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-(MediumDec self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*(MediumDec self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/(MediumDec self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend MediumDec operator+=(MediumDec& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend MediumDec operator-=(MediumDec& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend MediumDec operator*=(MediumDec& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDec operator/=(MediumDec& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-(MediumDec& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator ++()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                ++IntValue.Value;
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
            else
                ++IntValue.Value;
            return *this;
        }

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator --()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                --IntValue.Value;
            else if (IntValue.Value == 0)
                IntValue = NegativeRep;
            else
                --IntValue.Value;
            return *this;
        }

        /// <summary>
        /// MediumDec++ Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator ++(int)
        {
            MediumDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec-- Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator --(int)
        {
            MediumDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec* Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

    #pragma region Modulus Operations
    #if !defined(AltNum_PreventModulusOverride)

        friend MediumDec operator%(MediumDec& self, int Value) { return IntRemOp(self, Value); }
        friend MediumDec operator%(MediumDec& self, signed long long Value) { return IntRemOp(self, Value); }

        friend MediumDec operator%=(MediumDec& self, int Value) { return IntRemOp(self, Value); }
        friend MediumDec operator%=(MediumDec& self, signed long long Value) { return IntRemOp(self, Value); }

        friend MediumDec operator%=(MediumDec* self, int Value) { return IntRemOp(**self, Value); }
        friend MediumDec operator%=(MediumDec* self, signed long long Value) { return IntRemOp(**self, Value); }
        
        friend MediumDec operator%(MediumDec self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
    
        #if defined(AltNum_EnableAlternativeModulusResult)
        //friend MediumDec operator%(MediumDec& self, int Value) { return IntRemOp(self, Value); }
        //friend MediumDec operator%(MediumDec& self, signed long long Value) { return IntRemOp(self, Value); }
        //friend MediumDec operator%(MediumDec self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
        #endif
    #endif
    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        friend MediumDec operator^(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue ^= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf ^= Value;
                }
                else
                {
                    self.IntValue ^= Value; self.DecimalHalf ^= Value;
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
        template<IntegerType IntType=int>
        friend MediumDec operator|(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue |= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf |= Value;
                }
                else
                {
                    self.IntValue |= Value; self.DecimalHalf |= Value;
                }
            }
            return self;
        }
    #endif
    #pragma endregion Bitwise Functions

    }
    #pragma region String Function Source

    #pragma endregion String Function Source

#if defined(AltNum_EnableAlternativeModulusResult)
    /// <summary>
    /// (MediumDecBase Version)Performs remainder operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    class DLL_API AltModChecker : public AltNumModChecker<MediumDecBase>
    {
    };
#endif
}