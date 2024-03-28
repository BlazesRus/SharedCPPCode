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

#include "MirroredInt.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/

//"Not used for this variant" comment used as placeholder
// between unused regions to help with code compare between variants and keep structure similar

namespace BlazesRusCode
{
#if !defined(AltNum_UseIntForDecimalHalf)
	struct PartialInt {
	#pragma options align=bit_packed
	//Stores Digits XXX XXX XXX
	unsigned int Value:30;
	//Can store up to 4 Flag states including normal state at 0
	unsigned int Flags:2;
	#pragma options align=reset
		PartialInt(unsigned int value=0, unsigned int flags=0)
		{
			Value = value;
			Flags = flags;
		}
	};
#endif

    class MediumDecBase;

/*---Accuracy Tests(with MediumDecBase based settings):
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication and division because of truncation
   (values get lost if get too small) (100% accuracy except for some truncated digits lost)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like Pi(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API MediumDecBase
    {
#if !defined(AltNum_DisableDefaultStringFormatOption)
	enum class DefaultStringFormatEnum : int
	{
	
	}
#endif
	public:
#if defined(AltNum_EnableAlternativeModulusResult)
		class ModRes
		{
			//Division Result
			MediumDecBaseVariant DivRes;
			//Modulo Operation Result
			MediumDecBaseVariant RemRes;
		}
#endif
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;

	protected:
        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static MirroredInt NegativeRep;
        static signed int const NegativeRepVal = MirroredInt::NegativeRep;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        MirroredInt IntValue;

        signed int GetIntHalf()
        {
            return IntValue.GetValue();
        }

        bool IsNegative()
        {
            return IntValue.IsNegative();
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(int intVal, signed int decVal = 0)
        {
            IntValue.Value = intVal;
            DecimalHalf = decVal;
        }

        MediumDecBase(const MediumDecBase&) = default;

        MediumDecBase& operator=(const MediumDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        virtual bool IsAtZeroInt()
        {
            return IntValue==0||IntValue==NegativeRep;
        }

        virtual bool IsNotAtZeroInt()
        {
            return IntValue != 0 && IntValue != NegativeRep;
        }

        //Detect if at exactly zero
		virtual bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDecBase Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
        }
        
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            if (IntValue == NegativeRep)
            {
                IntValue = 0;
            }
            else if (IntValue == 0)
            {
                IntValue = NegativeRep;
            }
            else
            {
                IntValue *= -1;
            }
        }

    #pragma region Const Representation values
    protected:
	#if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
	#endif

    #if defined(AltNum_EnableNilRep)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif

    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: int
        {
            NormalType = 0,
	//#if defined(AltNum_EnableFractionals)
            NumByDiv = 8,
	//#endif
	//#if defined(AltNum_EnablePiRep)
            PiNum = 1,
		//#if defined(AltNum_EnablePiPowers)
            PiPower = 17,
		//#endif
		//#if defined(AltNum_EnableAlternativeRepFractionals)
			//#if defined(AltNum_EnableDecimaledPiFractionals)
            PiNumByDiv = 9,//  (Value/(ExtraRep*-1))*Pi Representation
			//#else
            PiFractional = 9,//  IntValue/DecimalHalf*Pi Representation
			//#endif
		//#endif
	//#endif
	//#if defined(AltNum_EnableERep)
            ENum = 2,
		//#if defined(AltNum_EnableAlternativeRepFractionals)
			//#if defined(AltNum_EnableDecimaledEFractionals)
            ENumByDiv = 10,//(Value/(ExtraRep*-1))*e Representation
			//#else
            EFractional = 10,//  IntValue/DecimalHalf*e Representation
			//#endif
		//#endif
	//#endif
	//#if defined(AltNum_EnableImaginaryNum)
            INum = 4,
		//#if defined(AltNum_EnableAlternativeRepFractionals)
			//#if defined(AltNum_EnableDecimaledIFractionals)
            INumByDiv = 11,//(Value/(ExtraRep*-1))*i Representation
			//#else
            IFractional = 11,//  IntValue/DecimalHalf*i Representation
			//#endif
		//#endif
		//#ifdef AltNum_EnableComplexNumbers
            ComplexIRep = 255,
		//#endif
	//#endif
	//#if defined(AltNum_EnableMixedFractional)
            MixedFrac = 32,//IntValue +- (-DecimalHalf)/ExtraRep
            //#if defined(AltNum_EnableMixedPiFractional)
            MixedPi = 33,//IntValue +- (-DecimalHalf/-ExtraRep)
		//#elif defined(AltNum_EnableMixedEFractional)
            MixedE = 34,//IntValue +- (-DecimalHalf/-ExtraRep)
		//#elif defined(AltNum_EnableMixedIFractional)
            MixedI = 36,//IntValue +- (-DecimalHalf/-ExtraRep)
		//#endif
	//#endif

	//#if defined(AltNum_EnableInfinityRep)
			PositiveInfinity = 192,//If Positive Infinity, then convert number into MaximumValue instead when need as real number
			NegativeInfinity = 112,//If Negative Infinity, then convert number into MinimumValue instead when need as real number
	//#endif
	//#if defined(AltNum_EnableApproachingValues)
            ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
		//#if !defined(AltNum_DisableApproachingTop)
            ApproachingTop = 72,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		//#endif
		//#if defined(AltNum_EnableApproachingDivided)
			ApproachingMidLeft = 80,//DecimalHalf:1000000000/ExtraRep - ApproachingZero (AlternativeName:ApproachingMidLeft)
			//#if !defined(AltNum_DisableApproachingTop)
            ApproachingMidRight = 96,//DecimalHalf:1000000000/ExtraRep + ApproachingZero (AlternativeName:ApproachingMidRight)
			//#endif
		//#endif
	//#endif
    //#if defined(AltNum_EnableNaN)
            Undefined = 128,
            NaN = 129,
    //#endif
	//#if defined(AltNum_EnableApproachingPi)
            ApproachingTopPi = 65,//equal to IntValue.9..9 Pi
	//#endif
	//#if defined(AltNum_EnableApproachingE)
            ApproachingTopE = 66,//equal to IntValue.9..9 e
	//#endif
	//#if defined(AltNum_EnableImaginaryInfinity)
            PositiveImaginaryInfinity = 196,
			NegativeImaginaryInfinity = 228,
	//#endif
	//#if defined(AltNum_EnableApproachingI)
            ApproachingImaginaryBottom = 196,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		//#if !defined(AltNum_DisableApproachingTop)
            ApproachingImaginaryTop = 228,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		//#endif
		//#if defined(AltNum_EnableApproachingDivided)
			ApproachingImaginaryMidLeft = 84,//DecimalHalf:1000000000/ExtraRep - ApproachingImaginaryZero
			//#if !defined(AltNum_DisableApproachingTop)
            ApproachingImaginaryMidRight = 100,//DecimalHalf:1000000000/ExtraRep + ApproachingImaginaryZero
			//#endif
		//#endif
    //#endif
	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            UndefinedButInRange = 130,
		//#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
			WithinMinMaxRange = 136,
		//#endif
	//#endif
    //#if defined(AltNum_EnableNilRep)
            Nil,
    //#endif
            UnknownType
        };

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType const GetRepType()
        {
            return RepType::UnknownType;//Virtual code replaced inside main class(Not actually used inside MediumDec class)
        }

    #pragma endregion RepType

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMaximum()
        {
            IntValue = 2147483647; DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
            IntValue = -2147483647; DecimalHalf = 999999999;
        }
	
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
	//Not used for this variant
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant
	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
        }
	#endif
	#pragma endregion NaN Setters

    #pragma region ValueDefines
	//Stored inside Actual MediumDecBase class to prevent deriving statics
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(std::string Value);

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecBase(const char* strVal)
        {
            std::string Value = strVal;
            if (Value == "Pi")
            {
                this->SetVal(Pi);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(std::string Value)
        {
            if (Value == "Pi")
            {
                this->SetVal(Pi);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

#pragma endregion String Commands

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
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetFloatVal(float Value)
        {
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { Value *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDoubleVal(double Value)
        {
            bool IsNegative = Value < 0.0;
            if (IsNegative) { Value *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDecimalVal(ldouble Value)
        {
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { Value *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0L)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (ldouble)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual SetBoolVal(bool Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(float Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(double Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(ldouble Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(bool Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecBaseBasedSetValues)
        MediumDecBase(MediumDecBase Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
        //Adding more exact conversion from floating point to MediumDecBase variant later

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual float toFloat()
        {
            float Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue.GetValue();
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
                Value = (float)IntValue.GetValue();
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDouble()
        {
            double Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue.GetValue();
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
                Value = (double)IntValue.GetValue();
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual ldouble toDecimal()
        {
            ldouble Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue.GetValue();
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (ldouble)IntValue.GetValue();
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }
		
        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual int toInt() { return IntValue.GetValue(); }

        virtual bool toBool() { return IntValue.IsZero() ? false : true; }
    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
	//Not used for this variant
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	//Not used for this variant
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
	//Not used for this variant
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
    std::strong_ordering operator<=>(const MediumDecBase& that) const
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

    bool operator==(const MediumDecBase& that) const
    {
        if (IntValue!=that.IntValue)
            return false;
        if (DecimalHalf!=that.IntValue)
            return false;
    }
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:
        template<typename IntType>
        void PartialIntDivOp(IntType& Value)
        {
            if (DecimalHalf == 0)
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                    IntValue *= -1;
                __int64 SRep = DecimalOverflowX * IntValue;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflow;
                    SRep -= OverflowVal * DecimalOverflow;
                    IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= DecimalOverflowX * OverflowVal;
                    IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }
public:

		void PartialDivOp(signed int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(unsigned int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(signed long long& Value) { PartialIntDivOp(Value); }
        void PartialDivOp(unsigned long long& Value) { PartialIntDivOp(Value); }

		static void PartialDivOp(MediumDecBase& self, signed int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(MediumDecBase& self, unsigned int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(MediumDecBase& self, signed long long& Value) { self.PartialIntDivOp(Value); }
        static void PartialDivOp(MediumDecBase& self, unsigned long long& Value) { self.PartialIntDivOp(Value); }

		MediumDecBase PartialDiv(signed int Value)
        { MediumDecBase self = *this; PartialIntDivOp(Value); return self; }
		MediumDecBase PartialDiv(unsigned int Value)
        { MediumDecBase self = *this; PartialIntDivOp(Value); return self; }
		MediumDecBase PartialDiv(signed long long Value)
        { MediumDecBase self = *this; PartialIntDivOp(Value); return self; }
        MediumDecBase PartialDiv(unsigned long long Value)
        { MediumDecBase self = *this; PartialIntDivOp(Value); return self; }

		static MediumDecBase PartialDiv(MediumDecBase& self, signed int Value) { self.PartialIntDivOp(Value); return self; }
		static MediumDecBase PartialDiv(MediumDecBase& self, unsigned int Value) { self.PartialIntDivOp(Value); return self; }
		static MediumDecBase PartialDiv(MediumDecBase& self, signed long long Value) { self.PartialIntDivOp(Value); return self; }
        static MediumDecBase PartialDiv(MediumDecBase& self, unsigned long long Value) { self.PartialIntDivOp(Value); return self; }

protected:
        template<typename IntType>
        MediumDecBase& BasicIntDivOp(IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }

        template<typename IntType>
        MediumDecBase& BasicUnsignedIntDivOp(IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }
public:

		virtual void BasicDivOp(signed int& Value) { BasicIntDivOp(Value); }
		virtual void BasicDivOp(unsigned int& Value) { BasicUnsignedIntDivOp(Value); }
		virtual void BasicDivOp(signed long long& Value) { BasicIntDivOp(Value); }
        virtual void BasicDivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }

        template<MediumDecVariant VariantType=MediumDecBase>
		virtual VariantType BasicDiv(signed int Value)
        { MediumDecBase self = *this; BasicIntDivOp(Value); return self; }
		
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType BasicDiv(unsigned int Value)
        { MediumDecBase self = *this; BasicUnsignedIntDivOp(Value); return self; }
		
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType BasicDiv(signed long long Value)
        { MediumDecBase self = *this; BasicIntDivOp(Value); return self; }
        
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType BasicDiv(unsigned long long Value)
        { MediumDecBase self = *this; BasicUnsignedIntDivOp(Value); return self; }

    #pragma endregion NormalRep Integer Division Operations
	
	#pragma region NormalRep AltNumToAltNum Operations
protected:
        //Return true if divide into zero
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual bool PartialDivOp(VariantType& Value)
        {
			bool ResIsPositive = true;
			signed _int64 SelfRes;
			signed _int64 ValueRes;
			if(IntValue<0)
			{
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
			        SelfRes = NegDecimalOverflowX*IntValue + DecimalHalf;
                if (Value < 0)
                {
                    if (Value.IntValue == NegativeRep)
                        ValueRes = Value.DecimalHalf;
                    else
                        ValueRes =  NegDecimalOverflowX* Value.IntValue + Value.DecimalHalf;
                }
				else
				{
				    ResIsPositive = false;
					ValueRes = DecimalOverflowX * Value.IntValue +Value.DecimalHalf;
				}
			}
			else
			{
                
				SelfRes = DecimalOverflowX* IntValue+DecimalHalf;
			    if(Value<0)
				{
				    ResIsPositive = false;
					ValueRes = Value.IntValue==NegativeRep ? DecimalHalf: NegDecimalOverflowX*IntValue +Value.DecimalHalf;
				}
				else
					ValueRes = DecimalOverflowX* Value.IntValue +Value.DecimalHalf;
			}
			
			signed _int64 IntHalfRes = SelfRes / ValueRes;
			signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntValue = IntHalfRes==0&&ResIsPositive==false?NegativeRep:IntHalfRes;
			DecimalHalf = DecimalRes;
			if(IntHalfRes==0&&DecimalRes==0)
				return true;
			else
				return false;
        }

public:
		
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicDivOp(VariantType& Value)
        {
			if (PartialDivOp(Value))//Prevent Dividing into nothing
				DecimalHalf = 1;
        }

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType BasicDiv(MediumDecBase Value)
        {
            MediumDecBase self = *this;
            self.BasicDivOp(Value);
            return self;
        }

        //void CatchAllDivision;

public:
        template<MediumDecVariant VariantType=MediumDecBase>
        bool RepToRepDivOp(RepType& LRep, RepType& RRep, const VariantType& self, const VariantType& Value)
        {
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType& DivOp(VariantType& Value) { BasicDivOp(Value); return *this; }

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType DivideAsCopy(VariantType Value) { VariantType self = *this; self.BasicDivOp(Value); return self; }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType& MultOp(VariantType& Value) { BasicMultOp(Value); return *this; }

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType MultipleAsCopy(VariantType Value) { VariantType self = *this; self.BasicMultOp(Value); return self; }

        /// <summary>
        /// Addition Operation
        /// </summary>
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType& AddOp(VariantType& Value) { BasicAddOp(Value); return *this; }


        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType AddAsCopy(VariantType Value) { VariantType self = *this; self.BasicAddOp(Value); return self; }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType& SubOp(VariantType& Value) { BasicSubOp(Value); return *this; }


        template<MediumDecVariant VariantType=MediumDecBase>
        virtual VariantType SubtractAsCopy(VariantType Value) { MediumDecBase self = *this; self.BasicSubOp(Value); return self; }

        //MediumDecBase& RemOp(VariantType& Value) { BasicRemOp(Value); return *this; }

        //MediumDecBase ModulusAsCopy(VariantType Value) { VariantType self = *this; self.BasicRemOp(Value); return self; }

    #pragma endregion NormalRep AltNumToAltNum Operations
	
    #pragma region Other Integer Operations
		/// <summary>
        /// Basic Multiplication Operation(main code block)
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
		virtual bool BasicMultOpPt2(VariantType& Value)
		{
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                }
                else if (Value.DecimalHalf == 0)
                {
                    IntValue *= Value.IntValue;
                }
                else
                {
                    Value.PartialIntMultOp(IntValue);
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                }
				return false;
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecBase::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
					return DecimalHalf==0?true:false;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
            }
            else if (IntValue == MediumDecBase::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecBase::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (Value.IntValue == 0)
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.DecimalHalf;
                    SRep /= MediumDecBase::DecimalOverflowX;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else if (Value.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.IntValue.GetValue();
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.IntValue.GetValue();//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(Value.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)Value.DecimalHalf;
                    Temp04 /= MediumDecBase::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDecBase::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDecBase::DecimalOverflow;
                    if (IntHalf == 0) { IntValue = (signed int)SelfIsNegative ? MediumDecBase::NegativeRep : 0; }
                    else { IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
            if(DecimalHalf==0&&IntValue==0)
                return true;
            else
                return false;
		}
		
		/// <summary>
        /// Basic Multiplication Operation(before ensuring doesn't multiply into nothing)
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
		virtual bool BasicMultOpPt1(VariantType& Value)
        {//Warning:Modifies Value to make it a positive variable
        //Only checking for zero multiplication in main multiplication method
        //Not checking for special representation variations in this method(closer to MediumDecBase operation code)
            if (Value.IntValue < 0)
            {
                Value.SwapNegativeStatus();
                SwapNegativeStatus();
            }
			return BasicMultOpPt2(Value);
        }

		/// <summary>
        /// Basic Multiplication Operation(without checking for special representation variations or zero)
		/// Returns true if prevented from multiplying into nothing(except when multipling by zero)
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
		virtual bool BasicMultOp(VariantType& Value)
		{
			if(BasicMultOpPt1(Value))//Prevent multiplying into zero
/*#if defined(AltNum_EnableApproachingValues)//Might adjust later to set to approaching zero in only certain situations(might be overkill to set to .0..1 in most cases)
			{	
				DecimalHalf = ApproachingBottomRep; ExtraRep = 0; 
			}
#else*/
				DecimalHalf = 1;
			else
				return false;
//#endif
			return true;
		}

        //BasicMultOp without negative number check
        template<MediumDecVariant VariantType=MediumDecBase>
		virtual bool BasicMultOpV2(MediumDecBase& Value)
		{
			if(BasicMultOpPt2(Value))//Prevent multiplying into zero
/*#if defined(AltNum_EnableApproachingValues)//Might adjust later to set to approaching zero in only certain situations(might be overkill to set to .0..1 in most cases)
			{	
				DecimalHalf = ApproachingBottomRep; ExtraRep = 0; 
			}
#else*/
				DecimalHalf = 1;
			else
				return false;
//#endif
			return true;
		}

protected:
		//void CatchAllMultiplication(MediumDecBase& Value, RepType& LRep, RepType& RRep)

public:		
		/// <summary>
        /// Partial Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        virtual void PartialIntMultOp(IntType& Value)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= Value;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        virtual void BasicIntMultOp(IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (Value == 0)
                SetAsZero();
            else
                PartialIntMultOp(Value);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value(Without negative flipping)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        virtual void BasicIntMultOpV2(IntType& Value)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (Value == 0)
                SetAsZero();
            else
                PartialIntMultOp(Value);
        }

        //IntMultOp without negative check
        template<MediumDecVariant VariantType=MediumDecBase, typename IntType>
        virtual VariantType& UnsignedIntMultOp(IntType& Value)
        {
            if (IsZero()||Value==1)
                return;
            if (Value == 0)
            {
                SetAsZero(); return;
            }
            IntMultOpPt2(Value);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<MediumDecVariant VariantType=MediumDecBase, typename IntType>
        virtual VariantType& IntMultOp(IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            if (IsZero()||Value==1)
                return;
            if (Value == 0)
            {
                SetAsZero(); return;
            }
            IntMultOpPt2(Value);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=int>
        virtual VariantType& UnsignedMultOp(const IntType& Value)
        {
            if (IsZero()) {}
            else if (Value == 0) { IntValue = 0; DecimalHalf = 0; }
            else if (DecimalHalf == 0)
            {
                IntValue *= Value;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep)
                        IntValue = 0;
                    else
                        IntValue *= -1;
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
            return this;
        }

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual void RepToRepMultOp(RepType& LRep, RepType& RRep, VariantType& self, VariantType& Value);
    #pragma endregion Multiplication/Division Operations

#pragma region Addition/Subtraction Operations
        /// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        virtual void BasicAddOp(VariantType& Value)
        {
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntValue += Value.IntValue;
            if (Value.DecimalHalf != 0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
        }

protected:

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual void CatchAllAddition(VariantType& Value, RepType& LRep, RepType& RRep)
        {
        }

        template<MediumDecVariant VariantType=MediumDecBase>
        virtual void RepToRepAddOp(RepType& LRep, RepType& RRep, VariantType& self, VariantType& Value);

public:
		/// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicSubOp(MediumDecBase& Value)
        {
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntValue -= Value.IntValue;
            //Now deal with the decimal section
            if(Value.DecimalHalf!=0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
        }

                /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        static MediumDecBase& IntSubOp(IntType& value)
        {
            if (value == 0)
                return;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert MediumDecBase into complex number at moment";
				return;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormType();
			bool WasNegative = IntValue < 0;
			IntValue += value;
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ IntValue >= 0)//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
			{
				if(DecimalHalf<0)//Flip the fractional half of mixed fraction if flips to other side
				{
		#if defined(AltNum_EnableAlternativeMixedFrac)
					if(ExtraRep<0)// DecimalHalf:-2,ExtraRep:-3 becomes DecimalHalf:-1, ExtraRep:-3
						DecimalHalf = ExtraRep - DecimalHalf;
					else
		#endif			
						DecimalHalf = -(ExtraRep+DecimalHalf);// DecimalHalf:-2,ExtraRep:3 becomes DecimalHalf:-1, ExtraRep:3
				}
				else
					DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
	#endif
            return;
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        static MediumDecBase& IntSubOp(MediumDecBase& self, IntType& value)
        {
            return self.IntSubOp(value);
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase&</returns>
        static MediumDecBase& SubOp(MediumDecBase& self, MediumDecBase& Value);

		//void CatchAllSubtraction(MediumDecBase& Value, RepType& LRep, RepType& RRep)


        //void RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecBase& self, MediumDecBase& Value);
#pragma endregion Addition/Subtraction Operations

    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase self, MediumDecBase Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& self, MediumDecBase Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(MediumDecBase self, MediumDecBase Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator*=(MediumDecBase& self, MediumDecBase Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(MediumDecBase self, MediumDecBase Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator+=(MediumDecBase& self, MediumDecBase Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase self, MediumDecBase Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& self, MediumDecBase Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(MediumDecBase self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between MediumDecBase and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase& operator+=(MediumDecBase& self, int Value) { return IntAddOp(self, Value); }

        //friend MediumDecBase operator+=(MediumDecBase* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& self, int Value) { return IntSubOp(self, Value); }

        //friend MediumDecBase& operator-=(MediumDecBase* self, int Value) { return IntSubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(MediumDecBase self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase& operator*=(MediumDecBase& self, int Value) { return IntMultOp(self, Value); }

        ///// <summary>
        ///// *= Operation Between MediumDecBase and Integer Value (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDecBase</returns>
        //friend MediumDecBase operator*=(MediumDecBase* self, int Value) { return IntMultOp(**self, Value); }

        /// <summary>
        /// Division Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& self, int Value) { return IntDivOp(self, Value); }

        //friend MediumDecBase operator/=(MediumDecBase* self, int Value) { return IntDivOp(**self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(MediumDecBase self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between MediumDecBase and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase& operator+=(MediumDecBase& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(MediumDecBase self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase operator*=(MediumDecBase& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/=(MediumDecBase& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend MediumDecBase operator+(MediumDecBase self, float Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(MediumDecBase self, float Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(MediumDecBase self, float Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(MediumDecBase self, float Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(float Value, MediumDecBase self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(float Value, MediumDecBase self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(float Value, MediumDecBase self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(float Value, MediumDecBase self) { return (MediumDecBase)Value / self; }

        friend MediumDecBase operator+(MediumDecBase self, double Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(MediumDecBase self, double Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(MediumDecBase self, double Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(MediumDecBase self, double Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(MediumDecBase self, ldouble Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(MediumDecBase self, ldouble Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(MediumDecBase self, ldouble Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(MediumDecBase self, ldouble Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(ldouble Value, MediumDecBase self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(ldouble Value, MediumDecBase self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(ldouble Value, MediumDecBase self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(ldouble Value, MediumDecBase self) { return (MediumDecBase)Value / self; }


        friend MediumDecBase operator+(MediumDecBase self, unsigned char Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-(MediumDecBase self, unsigned char Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*(MediumDecBase self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/(MediumDecBase self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend MediumDecBase operator+=(MediumDecBase& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-=(MediumDecBase& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*=(MediumDecBase& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/=(MediumDecBase& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend MediumDecBase operator+(MediumDecBase self, unsigned short Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-(MediumDecBase self, unsigned short Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*(MediumDecBase self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/(MediumDecBase self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend MediumDecBase operator+=(MediumDecBase& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-=(MediumDecBase& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*=(MediumDecBase& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/=(MediumDecBase& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend MediumDecBase operator+(MediumDecBase self, unsigned int Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-(MediumDecBase self, unsigned int Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*(MediumDecBase self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/(MediumDecBase self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        

        friend MediumDecBase operator+=(MediumDecBase& self, unsigned int Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-=(MediumDecBase& self, unsigned int Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*=(MediumDecBase& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/=(MediumDecBase& self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        
        friend MediumDecBase operator+(MediumDecBase self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-(MediumDecBase self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*(MediumDecBase self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/(MediumDecBase self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend MediumDecBase operator+=(MediumDecBase& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend MediumDecBase operator-=(MediumDecBase& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend MediumDecBase operator*=(MediumDecBase& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend MediumDecBase operator/=(MediumDecBase& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-(MediumDecBase& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator ++()
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
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator --()
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

    #pragma region Modulus Operations
    #if !defined(AltNum_PreventModulusOverride)

        friend MediumDecBase operator%(MediumDecBase& self, int Value) { return IntRemOp(self, Value); }
        friend MediumDecBase operator%(MediumDecBase& self, signed long long Value) { return IntRemOp(self, Value); }

        friend MediumDecBase operator%=(MediumDecBase& self, int Value) { return IntRemOp(self, Value); }
        friend MediumDecBase operator%=(MediumDecBase& self, signed long long Value) { return IntRemOp(self, Value); }

        friend MediumDecBase operator%=(MediumDecBase* self, int Value) { return IntRemOp(**self, Value); }
        friend MediumDecBase operator%=(MediumDecBase* self, signed long long Value) { return IntRemOp(**self, Value); }
        
        friend MediumDecBase operator%(MediumDecBase self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
    
        #if defined(AltNum_EnableAlternativeModulusResult)
        //friend MediumDecBase operator%(MediumDecBase& self, int Value) { return IntRemOp(self, Value); }
        //friend MediumDecBase operator%(MediumDecBase& self, signed long long Value) { return IntRemOp(self, Value); }
        //friend MediumDecBase operator%(MediumDecBase self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
        #endif
    #endif
    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase operator^(MediumDecBase self, IntType Value)
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
        /// Bitwise Or Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType>
        friend MediumDecBase operator|(MediumDecBase self, IntType Value)
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

	#pragma region Math Etc Functions
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& Abs()
        {
            if (IntValue == NegativeRep)
                IntValue.Value = 0;
            else if (IntValue.Value < 0)
                IntValue *= -1;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Abs(MediumDecBase tValue)
        {
            return tValue.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& Floor()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase&</returns>
        static MediumDecBase Floor(MediumDecBase Value)
        {
            return Value.Floor();
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static MediumDecBase Floor(MediumDecBase Value, int precision)
        {
            switch (precision)
            {
            case 9: break;
            case 8: Value.DecimalHalf /= 10; Value.DecimalHalf *= 10; break;
            case 7: Value.DecimalHalf /= 100; Value.DecimalHalf *= 100; break;
            case 6: Value.DecimalHalf /= 1000; Value.DecimalHalf *= 1000; break;
            case 5: Value.DecimalHalf /= 10000; Value.DecimalHalf *= 10000; break;
            case 4: Value.DecimalHalf /= 100000; Value.DecimalHalf *= 100000; break;
            case 3: Value.DecimalHalf /= 1000000; Value.DecimalHalf *= 1000000; break;
            case 2: Value.DecimalHalf /= 10000000; Value.DecimalHalf *= 10000000; break;
            case 1: Value.DecimalHalf /= 100000000; Value.DecimalHalf *= 100000000; break;
            default: Value.DecimalHalf = 0; break;
            }
            if (Value.IntValue == NegativeRep && Value.DecimalHalf == 0) { Value.DecimalHalf = 0; }
            return Value;
        }
        
        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& Ceil()
        {
            if (DecimalHalf != 0)
            {
                DecimalHalf = 0;
                if (IntValue == NegativeRep) { IntValue = 0; }
                else
                {
                    ++IntValue;
                }
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        static int FloorInt(MediumDecBase Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue.GetValue();
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.IntValue.GetValue() - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        static int CeilInt(MediumDecBase Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue.GetValue();
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.IntValue.GetValue() + 1;
            }
        }
        
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Ceil(MediumDecBase Value)
        {
            return Value.Ceil();
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& Trunc()
        {
            DecimalHalf = 0;
            return *this;
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Trunc(MediumDecBase Value)
        {
            return Value.Trunc();
        }
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions	
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecBase BasicSqrt(MediumDecBase& value, int precision=7)
        {//Ignores Alternate representations use Sqrt instead to check based on RepType
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntValue.GetValue())
                {
                case 1: value.IntValue = 1; break;
                case 4: value.IntValue = 2; break;
                case 9: value.IntValue = 3; break;
                case 16: value.IntValue = 4; break;
                case 25: value.IntValue = 5; break;
                case 36: value.IntValue = 6; break;
                case 49: value.IntValue = 7; break;
                case 64: value.IntValue = 8; break;
                case 81: value.IntValue = 9; break;
                case 100: value.IntValue = 10; break;
                case 121: value.IntValue = 11; break;
                case 144: value.IntValue = 12; break;
                case 169: value.IntValue = 13; break;
                case 196: value.IntValue = 14; break;
                case 225: value.IntValue = 15; break;
                case 256: value.IntValue = 16; break;
                case 289: value.IntValue = 17; break;
                case 324: value.IntValue = 18; break;
                case 361: value.IntValue = 19; break;
                case 400: value.IntValue = 20; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                {
                    return value;
                }
            }
            MediumDecBase number = value;
            MediumDecBase start = 0, end = number;
            MediumDecBase mid;

            // variable to store the answer 
            MediumDecBase ans;

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
            MediumDecBase increment = "0.1";
            for (int i = 0; i < precision; i++) {
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
		static MediumDecBase Sqrt(MediumDecBase value, int precision=7)
		{
		    value.ConvertToNormType();
			BasicSqrt(value, precision);
		}

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecBase BasicIntPowOp(ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                if (DecimalHalf == 0 && IntValue == 10 && expValue >= -9)
                {
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1];
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecBase self = *this;
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expValue % 2 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return this;
                }
            }
            else if (DecimalHalf == 0 && IntValue == 10 && ExtraRep == 0)
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            else if (DecimalHalf == 0 && IntValue == -10 && ExtraRep == 0)
                IntValue = expValue % 2 ? VariableConversionFunctions::PowerOfTens[expValue] : VariableConversionFunctions::PowerOfTens[expValue] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecBase self = *this;
                IntValue = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        MediumDecBase BasicPowOp(int& expValue) { return BasicIntPowOp(expValue); }
        MediumDecBase BasicPowOp(signed long long& expValue) { return BasicIntPowOp(expValue); }
        MediumDecBase BasicPow(int expValue) { return BasicIntPowOp(expValue); }
        MediumDecBase BasicPow(signed long long expValue) { return BasicIntPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecBase IntPowOp(MediumDecBase& targetValue, ValueType& expValue)
        {
            if (value.DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    targetValue.SetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    targetValue.SetAsZero();
                else if (targetValue.IntValue.Value == -1 && expValue % 2 == 0)
                    targetValue.IntValue.Value = 1;
                else
                    return targetValue;//Returns infinity
                return *this;
            }
            else
                targetValue.BasicIntPowOp(expValue);
            return targetValue;
        }

        static MediumDecBase PowOp(MediumDecBase& targetValue, int& expValue) { return IntPowOp(targetValue, expValue); }
        static MediumDecBase PowOp(MediumDecBase& targetValue, signed long long& expValue) { return IntPowOp(targetValue, expValue); }
        static MediumDecBase Pow(MediumDecBase targetValue, int expValue) { return IntPowOp(targetValue, expValue); }
        static MediumDecBase Pow(MediumDecBase targetValue, signed long long expValue) { return IntPowOp(targetValue, expValue); }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecBase BasicIntPowConstOp(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                if (DecimalHalf == 0 && IntValue == 10 && expValue >= -9)
                {
                    int expVal = expValue * -1;
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[expVal];
                }
                else
                {
                    int expVal = expValue;
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expVal *= -1;
                    MediumDecBase self = *this;
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expVal > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expVal % 2 == 1)
                            *this /= self;
                        // n must be even now
                        expVal = expVal >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return this;
                }
            }
            else if (DecimalHalf == 0 && IntValue == 10 && targetValue.ExtraRep == 0)
            {
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                int expVal = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecBase self = *this;
                IntValue = 1; DecimalHalf = 0;// Initialize result
                while (expVal > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expVal % 2 == 1)
                        this *= self;
                    // n must be even now
                    expVal = expVal >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        MediumDecBase BasicPowConstOp(const int& expValue) { return BasicIntPowConstOp(expValue); }
        MediumDecBase BasicPowConstOp(const signed long long& expValue) { return BasicIntPowConstOp(expValue); }
        MediumDecBase BasicPowConst(const int expValue) { return BasicIntPowConstOp(expValue); }
        MediumDecBase BasicPowConst(const signed long long expValue) { return BasicIntPowConstOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecBase IntPowConstOp(MediumDecBase& targetValue, const ValueType& expValue)
        {
            if (value.DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    targetValue.SetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    return Zero;
                else if (value.IntValue.Value == -1 && expValue % 2 == 0)
                    IntValue.Value = 1;
                else
                    return;//Returns infinity
                return *this;
            }
            else
                targetValue.BasicIntPowConstOp(expValue);
            return *this;
        }

        MediumDecBase PowConstOp(MediumDecBase& targetValue, const int& expValue) { return IntPowConstOp(targetValue, expValue); }
        MediumDecBase PowConstOp(MediumDecBase& targetValue, const long long& expValue) { return IntPowConstOp(targetValue, expValue); }
        MediumDecBase PowConst(MediumDecBase& targetValue, const int& expValue) { return IntPowConstOp(targetValue, expValue); }
        MediumDecBase PowConst(MediumDecBase& targetValue, const long long& expValue) { return IntPowConstOp(targetValue, expValue); }


        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase NthRoot(MediumDecBase value, int n, MediumDecBase precision = MediumDecBase::JustAboveZero)
        {
            MediumDecBase xPre = ((value - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            MediumDecBase delX = MediumDecBase(2147483647, 0);

            //  xK denotes current value of x 
            MediumDecBase xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (xPre * nMinus1 + value / MediumDecBase::Pow(xPre, nMinus1)) / n;
                delX = MediumDecBase::Abs(xK - xPre);
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
        static MediumDecBase FractionalPow(MediumDecBase value, int expNum, int expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static MediumDecBase FractionalPow(MediumDecBase& value, boost::rational<int>& Frac);

        void BasicPowOp(MediumDecBase& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase PowOp(MediumDecBase& expValue);

        static MediumDecBase PowOp(MediumDecBase& targetValue, MediumDecBase& expValue)
        {
            return targetValue.PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecBase Pow(MediumDecBase targetValue, MediumDecBase expValue)
        {
            return PowOp(targetValue, expValue);
        }
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static MediumDecBase NthRootV2(MediumDecBase targetValue, int n, MediumDecBase& Precision = MediumDecBase::FiveBillionth)
        {
            int nMinus1 = n - 1;
            MediumDecBase x[2] = { (MediumDecBase::One / n) * ((targetValue*nMinus1) + (targetValue / MediumDecBase::Pow(targetValue, nMinus1))), targetValue };
            while (MediumDecBase::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (MediumDecBase::One / n) * ((x[1]*nMinus1) + (targetValue / MediumDecBase::Pow(x[1], nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Exp(MediumDecBase& x)
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
             *      - x: (MediumDecBase float) power of e to evaluate
             * Returns:
             *      - (MediumDecBase float) approximation of e^x in MediumDecBase precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.IntValue && x.IntValue <= 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return MediumDecBase::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            MediumDecBase x0 = MediumDecBase::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            MediumDecBase tn = MediumDecBase::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = MediumDecBase::CeilInt(x0 * MediumDecBase::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + MediumDecBase::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = MediumDecBase::One / tn;
            }
            return tn;
        }
protected:
    static MediumDecBase LnRef_Part02(MediumDecBase& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
        MediumDecBase TotalRes = (value - 1) / (value + 1);
        MediumDecBase LastPow = TotalRes;
        MediumDecBase WSquared = TotalRes * TotalRes;
        MediumDecBase AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecBase::JustAboveZero);
        return TotalRes * 2;
    }
public:
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static MediumDecBase LnRef(MediumDecBase& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecBase::One)
                return MediumDecBase::Zero;
            if (IntValue>=0&&IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecBase threshold = MediumDecBase::FiveMillionth;
                MediumDecBase base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecBase term = base;       // First term
                MediumDecBase prev;          // Previous sum
                MediumDecBase result = term;     // Kick it off

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
                } while (MediumDecBase::Abs(prev - result) > threshold);

                return result;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
                return LnRef_Part02(value);
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static MediumDecBase LnRefV2(MediumDecBase& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecBase::One)
                return MediumDecBase::Zero;
            if(IntValue==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecBase W = (value - 1)/ (value + 1);
                MediumDecBase TotalRes = W;
                W.SwapNegativeStatus();
                MediumDecBase LastPow = W;
                MediumDecBase WSquared = W * W;
                int WPow = 3;
                MediumDecBase AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDecBase::JustAboveZero);
                return TotalRes * 2;
            }
            else if (IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecBase threshold = MediumDecBase::FiveMillionth;
                MediumDecBase base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecBase term = base;       // First term
                MediumDecBase prev;          // Previous sum
                MediumDecBase result = term;     // Kick it off

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
                } while (MediumDecBase::Abs(prev - result) > threshold);

                return result;
            }
            else
            {
                return LnRef_Part02(value);
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        static MediumDecBase Ln(MediumDecBase value)
        {
            return LnRef(value);
        }

protected:
    static MediumDecBase Log10_Part02(MediumDecBase& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        MediumDecBase TotalRes = (value - 1) / (value + 1);
        MediumDecBase LastPow = TotalRes;
        MediumDecBase WSquared = TotalRes * TotalRes;
        MediumDecBase AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecBase::JustAboveZero);
        return TotalRes * MediumDecBase::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Log10(MediumDecBase value)
        {
            if (value == MediumDecBase::One)
                return MediumDecBase::Zero;
            if (DecimalHalf == 0 && IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecBase(index, 0);
                }
                return MediumDecBase(9, 0);
            }
            if (IntValue>=0&&IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecBase threshold = MediumDecBase::FiveBillionth;
                MediumDecBase base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecBase term = base;       // First term
                MediumDecBase prev = 0;          // Previous sum
                MediumDecBase result = term;     // Kick it off

                while (MediumDecBase::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*MediumDecBase::LN10Mult;// result/MediumDecBase::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_Part02(value);
            }
        }

protected:
    template<typename ValueType>
    static MediumDecBase Log10_IntPart02(ValueType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        MediumDecBase TotalRes = MediumDecBase((value - 1), 0) / MediumDecBase((value + 1), 0);
        MediumDecBase LastPow = TotalRes;
        MediumDecBase WSquared = TotalRes * TotalRes;
        MediumDecBase AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecBase::JustAboveZero);
        return TotalRes * MediumDecBase::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename ValueType>
        static MediumDecBase Log10(ValueType value)
        {
            if (value == 1)
                return MediumDecBase::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecBase(index, 0);
                }
                return MediumDecBase(9, 0);
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
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Log(MediumDecBase value, MediumDecBase baseVal)
        {
            if (ConvertedVal == MediumDecBase::One)
                return MediumDecBase::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Log(MediumDecBase value, int baseVal)
        {
            //Calculate Base log first
            MediumDecBase baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDecBase(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDecBase(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDecBase((baseVal - 1), 0) / MediumDecBase((baseVal + 1), 0);
                MediumDecBase baseLastPow = baseTotalRes;
                MediumDecBase baseWSquared = baseTotalRes * baseTotalRes;
                MediumDecBase baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > MediumDecBase::JustAboveZero);
            }

            //Now calculate other log
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? MediumDecBase(index, 0) / (baseTotalRes * MediumDecBase::HalfLN10Mult): MediumDecBase(index, 0)/ baseTotalRes;
                }
                return lnMultLog? MediumDecBase(9, 0) / (baseTotalRes*MediumDecBase::HalfLN10Mult):MediumDecBase(9, 0)/baseTotalRes;
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecBase threshold = MediumDecBase::FiveBillionth;
                MediumDecBase base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecBase term = base;       // First term
                MediumDecBase prev = 0;          // Previous sum
                MediumDecBase result = term;     // Kick it off

                while (MediumDecBase::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return lnMultLog? result/baseTotalRes:(result*2)/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecBase W = (value - 1) / (value + 1);
                MediumDecBase TotalRes = W;
                MediumDecBase AddRes;
                int WPow = 3;
                do
                {
                    AddRes = MediumDecBase::Pow(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecBase::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * MediumDecBase::HalfLN10Mult)/ baseTotalRes;
            }
            //return Log10(Value) / Log10(BaseVal);
        }
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions
        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase SinFromAngle(MediumDecBase Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecBase::Zero; }
            else if (Value.IntValue == 30 && Value.DecimalHalf == 0)
            {
                return PointFive;
            }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return One;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                MediumDecBase NewValue = Zero;
                //Angle as Radian
                MediumDecBase Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += MediumDecBase::Pow(Radius, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        static MediumDecBase CosFromAngle(MediumDecBase Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return One; }
            else if (Value.IntValue == 60 && Value.DecimalHalf == 0)
            {
                return PointFive;
            }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Zero;
            }
            else
            {
                MediumDecBase NewValue = Zero;
                //Angle as Radian
                MediumDecBase Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's also Taylor series!!
                    NewValue += MediumDecBase::Pow(Radius, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        //static MediumDecBase BasicSinOperation(MediumDecBase& Value);

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Sin(MediumDecBase Value)
        {
            MediumDecBase SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += MediumDecBase::Pow(Value, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        //static MediumDecBase BasicCosOperation(MediumDecBase& Value);

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Cos(MediumDecBase Value)
        {
            MediumDecBase CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += MediumDecBase::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Tan(MediumDecBase Value)
        {
            MediumDecBase SinValue = Zero;
            MediumDecBase CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += MediumDecBase::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += MediumDecBase::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase TanFromAngle(MediumDecBase Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecBase::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Maximum;//Positive Infinity
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Minimum;//Negative Infinity
            }
            else
            {
                return Tan(Pi * Value / 180);
            }
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase ATan(MediumDecBase Value)
        {
            MediumDecBase SinValue = Zero;
            MediumDecBase CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += MediumDecBase::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += MediumDecBase::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
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
        /// <returns>MediumDecBase</returns>
        static MediumDecBase ArcTan2(MediumDecBase y, MediumDecBase x)
        {
            MediumDecBase coeff_1 = PiNum / 4;
            MediumDecBase coeff_2 = coeff_1 * 3;
            MediumDecBase abs_y = MediumDecBase::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            MediumDecBase r;
            MediumDecBase angle;
            if (x >= 0)
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y < 0)
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }
    #pragma endregion Math/Trigonomic Etc Functions
    };

	MirroredInt MediumDecBase::NegativeRep = MirroredInt::NegativeZero;
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
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
    #if defined(AltNum_EnableNilRep)
    MediumDecBase MediumDecBase::Nil = NilValue();
    #endif

    MediumDecBase MediumDecBase::PiNum = PiNumValue();
    MediumDecBase MediumDecBase::ENum = ENumValue();
    
#if defined(AltNum_EnableNaN)
    MediumDecBase MediumDecBase::NaN = NaNValue();
	MediumDecBase MediumDecBase::Undefined = UndefinedValue();
#endif
    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDecBase::ReadString(std::string Value)
    {
        IntValue = 0; DecimalHalf = 0;
        bool IsNegative = false;
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
                IsNegative = true;
            }
            else if (StringChar == '.')
            {
                ReadingDecimal = true;
            }
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
        if (IsNegative)
        {
            if (IntValue == 0) { IntValue = NegativeRep; }
            else { IntValue *= -1; }
        }
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>MediumDecBase</returns>
    inline MediumDecBase MediumDecBase::GetValueFromString(std::string Value)
    {
        MediumDecBase NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

	std::string MediumDecBase::BasicToStringOp()
	{
        std::string Value = (std::string)IntValue;
        if (DecimalHalf != 0)
        {
			unsigned __int8 CurrentDigit;
			std::string DecBuffer = "";
            Value += ".";
            int CurrentSection = DecimalHalf;
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

    std::string MediumDecBase::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::PositiveInfinity:
            return "∞";
            break;
        case RepType::NegativeInfinity:
            return "-∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp();
			break;
            #endif
        #endif
	#endif
    #if defined(AltNum_EnableFractionals)
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
    #endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToStringOp()+"π";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "∞i";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-∞i";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".9..9i";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingImaginaryMidRight:
        case RepType::ApproachingImaginaryMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp()+"i";
			break;
        #endif
            #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
		#endif
    #endif
	#if defined(AltNum_EnableNaN)
        case RepType::Undefined:
            return "Undefined";
        case RepType::NaN:
            return "NaN";
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        case UndefinedButInRange:
            return "UndefinedButInRange";
            break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange";
            break;
        #endif
	#endif
    #if defined(AltNum_EnableNilRep)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			ConvertToNormType(repType);
			return BasicToStringOp();
            break;
        }
    }

	std::string MediumDecBase::BasicToFullStringOp()
	{
        std::string Value = (std::string)IntValue;
        if (DecimalHalf != 0)
        {
			unsigned __int8 CurrentDigit;
            Value += ".";
            bool HasDigitsUsed = false;
            int CurrentSection = DecimalHalf;
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

    std::string MediumDecBase::ToFullString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::PositiveInfinity:
            return "∞";
            break;
        case RepType::NegativeInfinity:
            return "-∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp();
			#else
            return (std::string)IntValue + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp();
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            ConvertToNormType(repType);
			return BasicToFullStringOp();
			break;
            #endif
        #endif
	#endif
    #if defined(AltNum_EnableFractionals)
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
    #endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToFullStringOp()+"π";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToFullStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToFullStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "∞i";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-∞i";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp()+"i";
			#else
            return (std::string)IntValue + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"i";
			#else
            return (std::string)IntValue + ".9..9i";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingImaginaryMidRight:
        case RepType::ApproachingImaginaryMidLeft:
            ConvertToNormType(repType);
			return BasicToFullStringOp()+"i";
			break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
		#endif
    #endif
	#if defined(AltNum_EnableNaN)
        case RepType::Undefined:
            return "Undefined";
        case RepType::NaN:
            return "NaN";
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        case UndefinedButInRange:
            return "UndefinedButInRange";
            break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange";
            break;
        #endif
	#endif
    #if defined(AltNum_EnableNilRep)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			ConvertToNormType(repType);
			return BasicToFullStringOp();
            break;
        }
    }
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