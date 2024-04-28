// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "AltNumBase.h"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\AltNumModChecker.hpp"
#include "..\IntegerConcept.hpp"
#include "..\MediumDecVariantConcept.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/
#include "MediumDecPreprocessors.h"
#include "RepType.h"

//"Not used for this variant" comment used as placeholder
// between unused regions to help with code compare between variants and keep structure similar
//Use  template<MediumDecVariant VariantType=MediumDecBase>
//to template functions for reuse with VariantTypes

namespace BlazesRusCode
{
    class MediumDecBase;
	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;
	
	// Forward declare VTable instances.
	//extern AltNum_VTable MediumDecBase_vtable;

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
    class DLL_API MediumDecBase : public virtual AltNumBase
    {
protected:
#if !defined(AltNum_DisableDefaultStringFormatOption)
		enum class DefaultStringFormatEnum : int
		{
	
		}
#endif
		//BitFlag 01(1) = PiRep
		RepTypeUnderlayer PiFlag = 1;
		//BitFlag 02(2) = ERep
		RepTypeUnderlayer EFlag = 2;
		//BitFlag 03(4) = IRep
		RepTypeUnderlayer IFlag = 4;
		//BitFlag 04(8) = Fractional Rep
		RepTypeUnderlayer FractionalFlag = 8;
		//If BitFlag#7 is set and BitFlag#6 isn't set,
		//then Approaching From Top side; 
		RepTypeUnderlayer ApproachFromRFlag = 8;
		//BitFlag 05 (16) = Power of flag
		RepTypeUnderlayer ToPowerOfFlag = 16;
		//If BitFlag#7 is set,
		//then Approaching but divided by ExtraRep
		RepTypeUnderlayer DividedByFlag = 16;
		//BitFlag 06 (32)= Mixed Fraction flag
		RepTypeUnderlayer MixedFracFlag = 32;
		//If BitFlag#7 is set and BitFlag#06 is set,
		//then Is Infinity Type
		RepTypeUnderlayer InfinityFlag = 32;
		//BitFlag 07 = Infinitesimal/Infinity Bit (Infinity or approaching representation)
		RepTypeUnderlayer InfTypeFlag = 64;
		//Bitflag 08= Undefined/NaN/Nil
		RepTypeUnderlayer UndefinedBit = 128;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;

        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
		
		//Maximum IntValue that can be stored inside IntValue field
	#if defined(AltNum_EnableMirroredSection)
        static MirroredInt const MaxIntValue;
	#else
        static signed int const MaxIntValue = 2147483647;
	#endif
	
		//Minimum IntValue that can be stored inside IntValue field
	#if defined(AltNum_EnableMirroredSection)
        static MirroredInt const MinIntValue;
	#else
        static signed int const MinIntValue = -2147483647;
	#endif
	public:

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
	#if defined(AltNum_EnableMirroredSection)
        static MirroredInt const NegativeRep;
	#else
        static signed int const NegativeRep = -2147483648;
	#endif

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        IntHalfType IntHalf;

		//Return IntHalf as signed int
        signed int GetIntHalf()
        {
	#if defined(AltNum_EnableMirroredSection)
			return IntValue.IsNegative()?((signed int)IntValue.Value)*-1:(signed int)IntValue.Value;
	#else
            return IntValue;
	#endif
        }

        bool IsNegative()
        {
	#if defined(AltNum_EnableMirroredSection)
            return IntValue.IsNegative();
	#else
			return IntValue<0;
	#endif
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        DecimalHalfType DecimalHalf;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const IntHalfType& intVal = 0, const DecimalHalfType& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        MediumDecBase(const MediumDecBase&) = default;

        MediumDecBase& operator=(const int& rhs)
        {
	#if defined(AltNum_EnableMirroredSection)
			if(rhs<0)
			{
				IntValue.Value = -rhs;
				IntValue.IsPositive = 0;
			}
			else
	#endif
				IntValue = rhs;
			DecimalHalf = 0;
            return *this;
        } const

        MediumDecBase& operator=(const MediumDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
	#if defined(AltNum_EnableMirroredSection)
            return IntValue.Value==0;
    #else
            return IntValue==0||IntValue==NegativeRep;
    #endif
        }

        bool IsNotAtZeroInt()
        {
	#if defined(AltNum_EnableMirroredSection)
            return IntValue.Value!=0;
    #else
            return IntValue != 0 && IntValue != NegativeRep;
    #endif
        }

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void SetVal(VariantType Value)
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
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsPositive ^= 1;
    #else
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
    #endif
        }

    #pragma region Const Representation values
    protected:

    #pragma endregion Const Representation values

    #pragma region RepType
/*
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType const GetRepType()
        {
            return RepType::UnknownType;//Virtual code replaced inside main class(Not actually used inside MediumDec class)
        }
*/

    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
            IntValue = MaxIntValue;
			DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
            IntValue = MinIntValue;
			DecimalHalf = 999999999;
        }
	
    #pragma endregion RangeLimits

	#pragma region PiNum Setters
	//Not used for this variant(Used in MediumDecBaseV2 and others)
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
	//Not used for this variant(Used in MediumDecBaseV2 and others)
	#pragma endregion ENum Setters

    #pragma region INum Setters
    //Not used for this variant(Used in MediumDecBaseV2 and others)
    #pragma endregion INum Setters

	#pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion Fractional Setters
        
	#pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion MixedFrac Setters
		
	#pragma region Infinity Setters
	//Not used for this variant(Used in MediumDecBaseV2 and others)
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant(Used in MediumDecBaseV2 and others)
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
	//Stored inside Actual MediumDec class to prevent deriving statics
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
        /// <returns>MediumDecBase</returns>
        template<MediumDecVariant VariantType=MediumDecBase>
        VariantType GetValueFromString(std::string Value)
        {

        }

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
        MediumDecBase(const std::string& Value)
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
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDoubleVal(const double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			double lValue = Value;
            bool IsNegative = Value < 0.0;
            if (IsNegative) { lValue *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDecimalVal(const ldouble& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			ldouble lValue = Value;
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { lValue *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (lValue >= 2147483648.0L)
            {
                if (IsNegative)
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(lValue);
                lValue -= (ldouble)WholeValue;
                DecimalHalf = (signed int)lValue * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetBoolVal(const bool& Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntVal(const int& Value)
        {
	#if defined(AltNum_EnableMirroredSection)
			if(Value<0)
			{
				IntValue.IsPositive = 0;
				IntValue.Value = -Value;
			}
			else
	#else
				IntValue = Value;
	#endif
			DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecBaseBasedSetValues)
        MediumDecBase(const MediumDecBase& Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
protected://Adding more exact conversion from floating point to MediumDecBase variant later

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void float toFloatV1()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            float Value;
            if (IntValue.IsNegative())
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
        #else
                Value = (float)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (float)IntValue;
        #else
                Value = (float)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
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
            if (IntValue < 0)
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0f : (double)IntValue;
        #else
                Value = (double)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (double)IntValue;
        #else
                Value = (double)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
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
            if (IntValue < 0)
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0L : (float)IntValue;
        #else
                Value = (ldouble)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (ldouble)IntValue;
        #else
                Value = (ldouble)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
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
        virtual float toFloat()
        {
            return toFloatV1;
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDouble()
        {
            return toDoubleV1();
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDecimal()
        {
            return toDecimalV1;
        }
		
        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual int toInt() { return IntValue; }

        virtual bool toBool() { return IntValue.IsZero() ? false : true; }

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

    #pragma region Pi Conversion
	//Not used for this variant
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	//Not used for this variant
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
    //Functions only used in variants of MediumDec

/*      //To-Do: Test cost of making virtual vs alternative means of making sure uses updated code after derivation    

        virtual void ConvertToNormType(const RepType& repType){}

protected:
		//Returns value as normal type representation
        template<MediumDecVariant VariantType=MediumDecBase>
        VariantType ConvertAsNormTypeV1(const RepType& repType)
        {
            VariantType Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        }
*/
		
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormType = MediumDecBase::ConvertAsNormTypeV1<MediumDecBase>;

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
*/

protected:
		////Returns value as normal type representation
  //      template<MediumDecVariant VariantType=MediumDecBase>
  //      VariantType AutoConvertAsNormType()
  //      {
  //          VariantType Res = *this;
  //          Res.ConvertToNormTypeV2();
  //          return Res;
  //      }
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormTypeV2 = MediumDecBase::AutoConvertAsNormType<MediumDecBase>;
*/
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal; int rVal;
	#else
			int lVal = IntValue==NegativeZero?0:IntValue;
			int rVal = that.IntValue==NegativeZero?0:that.IntValue;
	#endif
			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			lVal = IsNegative()?0-DecimalHalf:DecimalHalf;
			rVal = that.IsNegative()?0-that.DecimalHalf:that.DecimalHalf;
	#else
			lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
	#endif
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonWithoutSignCheck(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
				return IntHalfCmp;

			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			int lVal = IsNegative()?0-DecimalHalf:DecimalHalf;
			int rVal = that.IsNegative()?0-that.DecimalHalf:that.DecimalHalf;
	#else
			int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
	#endif
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
#endif
	
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = MediumDecBase::BasicComparisonV1<MediumDecBase>;
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = MediumDecBase::BasicComparisonWithoutSignCheck<MediumDecBase>;
#endif
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntValue <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal;
	#else
			int lVal = IntValue==NegativeZero?0:IntValue;
	#endif
			if (auto IntHalfCmp = lVal <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			lVal = DecimalHalf>0?1:0;
	#else
			lVal = DecimalHalf.Value>0?1:0;
	#endif
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		std::strong_ordering BasicIntComparisonV2(const int& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			int lVal = DecimalHalf>0?1:0;
	#else
			int lVal = DecimalHalf.Value>0?1:0;
	#endif
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
#endif
	
public:

		std::strong_ordering operator<=>(const MediumDecBase& that) const
		{
			return BasicComparison(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
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
        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOpV1(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
#if !defined(AltNum_EnableMirroredSection)
            bool ResIsNegative = IntValue < 0;
#endif
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
#if !defined(AltNum_EnableMirroredSection)
            if(DecimalHalf == 0)
                SelfRes = ResIsNegative?NegDecimalOverflowX * IntValue:DecimalOverflowX * IntValue;
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
    #if defined(AltNum_UseIntForDecimalHalf)
                        SelfRes = DecimalHalf;
    #else
                        SelfRes = DecimalHalf.Value;
    #endif
                    else
    #if defined(AltNum_UseIntForDecimalHalf)
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
    #else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf.Value;
    #endif
                }
                else
    #if defined(AltNum_UseIntForDecimalHalf)
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
    #else
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf.Value;
    #endif
            }
#else
    #if defined(AltNum_UseIntForDecimalHalf)
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntValue.Value: DecimalOverflowX * IntValue.Value + DecimalHalf;
    #else
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntValue.Value: DecimalOverflowX * IntValue.Value + DecimalHalf.Value;
    #endif
#endif
            Res = SelfRes / rValue;
            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
#if !defined(AltNum_EnableMirroredSection)
			if(ResIsNegative)
			{
				IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
				DecimalHalf = (int) DecimalRes;
			}
			else
			{
				IntValue = (int)IntHalfRes;
				DecimalHalf = DecimalRes;
			}
#else
		    IntValue.Value = (unsigned int)IntHalfRes;
    #if defined(AltNum_UseIntForDecimalHalf)
			DecimalHalf = (signed int)DecimalRes;
    #else
			DecimalHalf.Value = (unsigned int)DecimalRes;
    #endif
#endif
        }

        template<IntegerType IntType=signed int>
        void PartialIntDivOpV1(const IntType& Value)
        {
            if(Value<0)
            {
#if defined(AltNum_EnableMirroredSection)
                SwapNegativeStatus();
#endif
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
        }

public:
		
        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<const unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<const signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<const signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<const unsigned long long>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<const signed long long>;

protected:
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=signed int>
        VariantType& BasicUIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialUIntDivOpV1(Value);
            if (IntValue == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
            return *this;
        }
		
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=signed int>
        VariantType& BasicIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOpV1(Value);
            if (IntValue == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
            return *this;
        }
public:

        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<MediumDecBase, const unsigned int>;
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<MediumDecBase, const signed int>;
        constexpr auto UnsignedBasicIntDivOp = BasicUIntDivOpV1<MediumDecBase, const signed int>;
        constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<MediumDecBase, const unsigned long long>;
        constexpr auto BasicInt64DivOp = BasicIntDivOpV1<MediumDecBase, const signed long long>;

    #pragma endregion NormalRep Integer Division Operations
		
    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial version of BasicUIntMultOpV1 without zero checks
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType=int>
        void PartialUIntMultOpV1(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                IntValue *= rValue;
            else
			{
		#if !defined(AltNum_EnableMirroredSection)
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
			#if defined(AltNum_UseIntForDecimalHalf)
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
			#else
                __int64 SRep = IntValue == 0 ? DecimalHalf.Value : DecimalOverflowX * IntValue + DecimalHalf.Value;
			#endif
        #else
			#if defined(AltNum_UseIntForDecimalHalf)
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue.Value + DecimalHalf;
			#else
                __int64 SRep = IntValue == 0 ? DecimalHalf.Value : DecimalOverflowX * IntValue.Value + DecimalHalf.Value;
			#endif
        #endif
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
		#if defined(AltNum_EnableMirroredSection)
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
        #else
                    IntValue.Value = (unsigned int)OverflowVal;
        #endif
		#if defined(AltNum_UseIntForDecimalHalf)
                    DecimalHalf = (signed int)SRep;
		#else
                    DecimalHalf.Value = (unsigned int)SRep;
		#endif
                }
                else
                {
		#if !defined(AltNum_EnableMirroredSection)
                    IntValue = SelfIsNegative ? NegativeRep : 0;
		#else
					IntValue.Value = 0;
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
                    DecimalHalf = (signed int)SRep;
		#else
                    DecimalHalf.Value = (unsigned int)SRep;
		#endif
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
#if defined(AltNum_EnableMirroredSection)
                SwapNegativeStatus();
#endif
                PartialUIntMultOp(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
public:
		
        constexpr auto PartialUIntMultOp = PartialUIntMultOpV1<const unsigned int>;
        constexpr auto PartialIntMultOp = PartialIntMultOpV1<const signed int>;
        constexpr auto UnsignedPartialIntMultOp = PartialUIntMultOpV1<const signed int>;
        constexpr auto PartialUInt64MultOp = PartialUIntMultOpV1<const unsigned long long>;
        constexpr auto PartialInt64MultOp = PartialIntMultOpV1<const signed long long>;
		
protected:
        template<IntegerType IntType=signed int>
        auto& BasicUIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
            PartialUIntMultOpV1(Value);
            return *this;
        }
		
        template<IntegerType IntType=signed int>
        auto& BasicIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
			PartialIntMultOpV1(Value);
            return *this;
        }
public:

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<const unsigned int>;
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<const signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<const signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<const unsigned long long>;
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<const signed long long>;
		
    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:
    #if !defined(AltNum_EnableMirroredSection)
        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=signed int>
        void NRepSkippingIntAddOp(const IntType& rValue)
        {
            if (RValue == 0)
                return;
            if (IntValue == 0)
                IntValue = (int)rValue;
            else
                IntHalfAdditionOp(rValue);
            return;
        }
    #endif
public:

        /// <summary>
        /// Basic Addition Operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        auto& BasicIntAddOp(const IntType& rValue)
        {
    #if !defined(AltNum_EnableMirroredSection)
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(rValue);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfAdditionOp(rValue);
                //If flips to other side of negative, invert the decimal section
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
			}
    #else
			if(IntValue.IsPositive())
			{
				if(rValue<0)
				{
					IntType invertedrValue = -rValue;
					if(invertedrValue>(IntType)IntValue.Value)//Flips to other side of negative
					{
						IntValue.Value = rValue - IntValue + 1;
						if(DecimalHalf!=0)//Invert the decimal section
							DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
					}
					else
						IntValue += rValue;
				}
				else
					IntValue += rValue;
			}
			else
			{
				if(rValue<0)
					IntValue.Value -= -rValue;
				else
				{
					IntValue invertedValue = -IntValue.Value;
					if(invertedValue>=rValue)
						IntValue -= -rValue;
					else//Flips to other side of negative
					{
						IntValue.Value = rValue - IntValue + 1;
						if(DecimalHalf!=0)//Invert the decimal section
							DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
					}
				}
			}
    #endif
            return *this;
        }

        /// <summary>
        /// Basic Addition Operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOp(const IntType& rValue)
        {
    #if !defined(AltNum_EnableMirroredSection)
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(rValue);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfAdditionOp(rValue);
                //If flips to other side of negative, invert the decimal section
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
			}
    #else
			if(IntValue.IsPositive())
					IntValue += rValue;
			else
			{
				IntValue invertedValue = -IntValue.Value;
				if(invertedValue>=rValue)
					IntValue -= -rValue;
				else//Flips to other side of negative
				{
					IntValue.Value = rValue - IntValue + 1;
					if(DecimalHalf!=0)//Invert the decimal section
						DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
				}
			}
    #endif
            return *this;
        }

        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<MediumDecBase, const unsigned int>;
        constexpr auto BasicIntAddOp = BasicIntAddOpV1<MediumDecBase, const signed int>;
        constexpr auto UnsignedBasicIntAddOp = BasicUIntAddOpV1<MediumDecBase, const signed int>;
        constexpr auto BasicUInt64AddOp = BasicUIntAddOpV1<MediumDecBase, const unsigned long long>;
        constexpr auto BasicInt64AddOp = BasicIntAddOpV1<MediumDecBase, const signed long long>;
		
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
    #if !defined(AltNum_EnableMirroredSection)
        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=signed int>
        void NRepSkippingIntSubOp(const IntType& rValue)
        {
            if (RValue == 0)
                return;
            if (IntValue == 0)
                IntValue = -(int)rValue;
            else
                IntHalfSubtractionOp(rValue);
            return;
        }
    #endif
public:

		/// <summary>
        /// Basic Subtraction Operation between MediumDecBase and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        auto BasicIntSubOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
    #if !defined(AltNum_EnableMirroredSection)
                NRepSkippingIntSubOp(Value);
    #else
            {
                if(IntValue.IsNegative())
                {
                    if(rValue<0)
                    {
                        //Add code here
                    }
                    else
                        IntValue += -rValue;
                }
                else
                {
                    if(rValue<0)
                        IntValue.Value -= rValue;
                    else
                    {
                        //Add code here
                    }
                }
            }
    #endif
            else
            {
    #if !defined(AltNum_EnableMirroredSection)
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfSubtractionOp(rValue);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
    #else
                //Add code here
    #endif
            }
            return *this;
        }

        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<MediumDecBase, const unsigned int>;
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<MediumDecBase, const signed int>;
        constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<MediumDecBase, const signed int>;
        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<MediumDecBase, const unsigned long long>;
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<MediumDecBase, const signed long long>;

	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum Division Operations
protected:
	
public:

        constexpr auto BasicDivOp = BasicDivOpV1<MediumDecBase>;

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region NormalRep AltNum Multiplication Operations
		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned MediumDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedMultOp(const auto& rValue)
		{//To-Do:Update this code more towards current default format
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
                    __int64 rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                    bool ResIsNegative = IntValue<0;
                    if(ResIsNegative)
                    {
                        ResIsNegative = false;
                        rRep *= IntHalfAsAbs();
                    }
                    else
                    {
                        ResIsNegative = true;
                        rRep *= IntValue;
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            DecimalHalf = 1;
#endif
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
	#if !defined(AltNum_EnableMirroredSection)
            else if (IntValue == 0)
    #else
            else if (IntValue.Value == 0)
    #endif
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= MediumDecBase::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #endif
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
	#if !defined(AltNum_EnableMirroredSection)
            else if (IntValue == MediumDecBase::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= MediumDecBase::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                    if(DecimalHalf==0)
                    {
                        IntValue = 0;
                        if(ExtraRep!=0)
                            ExtraRep = 0;
                    }
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
                        return *this;
                    }
                }
            }
    #endif
            else
            {
	#if !defined(AltNum_EnableMirroredSection)
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
    #endif
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
	#if !defined(AltNum_EnableMirroredSection)
                        IntValue = (signed int)OverflowVal;
	#else
                        IntValue.Value = (unsigned int)OverflowVal;
	#endif
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
				#if !defined(AltNum_EnableMirroredSection)
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
				#else
                        IntValue.Value = 0;
				#endif
                    }
				    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= MediumDecBase::DecimalOverflowX;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
				#if !defined(AltNum_EnableMirroredSection)
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
				#else
                        IntValue.Value = (unsigned int)OverflowVal;
				#endif
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
				#if !defined(AltNum_EnableMirroredSection)
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
				#else
                        IntValue.Value = 0;
				#endif
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= MediumDecBase::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDecBase::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDecBase::DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

        void BasicMultOp(const IntType& Value)
        {
            if(Value<0)
            {
#if defined(AltNum_EnableMirroredSection)
                SwapNegativeStatus();
#endif
                BasicUnsignedMultOp(-Value);
            }
            else
                BasicUnsignedMultOp(Value);
        }

	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
protected:
	
public:
/*
        constexpr auto BasicAddOp = BasicAddOpV1<MediumDecBase>;
*/
	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicSubOp = BasicSubOpV1<MediumDecBase>;
*/
	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Mixed Fraction Operations
	//Used only in AltDec/MixedDec
	#pragma endregion Mixed Fraction Operations

	#pragma region Other Division Operations

		//Simplified division by 2 operation(to reduce cost of dividing)
        void DivideByTwo()
        {
            if(IntValue^1==1)//Check if number is odd
                rValue.BasicIntDivOp(2);
            else
                IntValue /= 2;
        }

	#pragma endregion Other Division Operations	

	#pragma region Other Multiplication Operations

		//Simplified multiplication by 2 operation(to reduce cost of multiplication)
        void MultipleByTwo()
        {
			//Add Code 
        }

	#pragma endregion Other Multiplication Operations

	#pragma region Other Addition Operations

	#pragma endregion Other Addition Operations

	#pragma region Other Subtraction Operations

	#pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
protected:
protected:
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=signed int>
        VariantType& BasicUIntModOpV1(const IntType& Value)
        {
			//To-Do:Update Code
            PartialUIntModOp(Value);
            return *this;
        }
		
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=signed int>
        VariantType& BasicIntModOpV1(const IntType& Value)
        {
			//To-Do:Update Code
			PartialIntModOp(Value);
            return *this;
        }
public:
/*
        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<MediumDecBase, const unsigned int&>;
		
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<MediumDecBase, const signed int&>;

        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<MediumDecBase, const unsigned long long&>;
		
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<MediumDecBase, const signed long long&>;
		
        constexpr auto BasicModOp = BasicModOpV1<MediumDecBase>;
*/
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
    //Update code later
    /*
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType=signed int>
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
        template<IntegerType IntType=signed int>
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
    */
	#pragma endregion Bitwise Operations

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
        template<IntegerType IntType=signed int>
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
        template<IntegerType IntType=signed int>
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
        template<IntegerType IntType=signed int>
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
        template<IntegerType IntType=signed int>
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
            if (DecimalHalf == 0)
                ++IntValue;
#if !defined(AltNum_EnableMirroredSection)
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
#endif
            else
                ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator --()
        {
            if (DecimalHalf == 0)
                --IntValue;
#if !defined(AltNum_EnableMirroredSection)
            else if (IntValue == 0)
                IntValue = NegativeRep;
#endif
            else
                --IntValue;
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

// Static versions of functions for Full versions
	#pragma region Math Etc Functions

/*
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
                return Value.IntValue;
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.IntValue - 1;
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
                return Value.IntValue;
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.IntValue + 1;
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
*/
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

/*
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecBase BasicSqrt(MediumDecBase& value, int precision=7)
        {//Ignores Alternate representations use Sqrt instead to check based on RepType
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntValue)
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
*/
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modifier owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumDecBase</returns>
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
             *      - approximation of e^x in MediumDecBase precision
             */
             // Check that x is a valid input.
#if !defined(AltNum_EnableMirroredSection)
            assert(-709 < IntValue && IntValue < 709);
#else
            assert(IntValue.Value < 709);
#endif
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

/*
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
*/
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions

/*
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
*/
    #pragma endregion Trigonomic Etc Functions
    };

    #pragma region ValueDefine Source
#if defined(AltNum_EnableMirroredSection)
	MirroredInt MediumDec::NegativeRep = MirroredInt::NegativeZero;
	MirroredInt MediumDec::MaxIntValue = MirroredInt::Maximum;
	MirroredInt MediumDec::MinIntValue = MirroredInt::Minimum;
#endif
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
#if !defined(AltNum_EnableMirroredSection)
        signed int CurrentSection;
#else
        unsigned int CurrentSection = IntValue.Value;
#endif
        unsigned __int8 CurrentDigit;
        if (IsNegative())
        {
            Value += "-";
#if !defined(AltNum_EnableMirroredSection)
            if (IntValue == NegativeRep)
				CurrentSection = 0; }
            else
				CurrentSection = -IntValue;
#endif
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
#if !defined(AltNum_EnableMirroredSection)
        signed int CurrentSection;
#else
        unsigned int CurrentSection = IntValue.Value;
#endif
        unsigned __int8 CurrentDigit;
        if (IsNegative())
        {
            Value += "-";
#if !defined(AltNum_EnableMirroredSection)
            if (IntValue == NegativeRep)
				CurrentSection = 0; }
            else
				CurrentSection = -IntValue;
#endif
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