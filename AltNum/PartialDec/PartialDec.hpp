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

    class PartialDec;

    /// <summary>
    /// Reduced version of AltDec that only adds feature of Fractionals to MediumDec
	/// and removes support for negative numbers
	/// </summary>
    class DLL_API PartialDec : AltNumBase
    {
	public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;
		
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        unsigned int IntHalf;

		//Return IntHalf as signed int
        unsigned int GetIntHalf()
        {
			return IntHalf;
        }

        bool IsNegative()
        {
			return false;
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        unsigned int DecimalHalf;

        //Divisor of Fractional
        unsigned int ExtraRep;

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        PartialDec(const unsigned int& intVal = 0, const unsigned int& decVal = 0, const unsigned int& extraRep = 0,)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraRep;
        }

        PartialDec(const PartialDec&) = default;

        PartialDec& operator=(const unsigned int& rhs)
        {
			IntValue = rhs;
			DecimalHalf = 0;
            ExtraRep = 0;
            return *this;
        } const

        PartialDec& operator=(const PartialDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = rhs.ExtraRep;
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
        void SetVal(const PartialInt& Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = Value.ExtraRep;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
        }
        
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus(){}

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
        virtual void SetAsMaximum()
        {
            IntValue = 2147483647;
			DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
		#if defined(AltNum_EnableMirroredSection)
			IntValue = MirroredInt(2147483647,0);
		#else
            IntValue = -2147483647;
		#endif
			DecimalHalf = 999999999;
        }
	
    #pragma endregion RangeLimits

	#pragma region PiNum Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion ENum Setters

    #pragma region INum Setters
    //Not used for this variant(Used in PartialDecV2 and others)
    #pragma endregion INum Setters

	#pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion Fractional Setters
        
	#pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion MixedFrac Setters
		
	#pragma region Infinity Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
    //Not supported by this variant
	#pragma endregion NaN Setters

    #pragma region ValueDefines

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
        /// <returns>PartialDec</returns>
        template<MediumDecVariant VariantType=PartialDec>
        VariantType GetValueFromString(std::string Value)
        {

        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        PartialDec(const char* strVal)
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
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const std::string& Value)
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
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value)
        {
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
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const ldouble& Value)
        {
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
        void SetIntVal(const int& Value)
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
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

        PartialDec(const PartialDec& Value)
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
        int toInt() {
            auto lValue = *this;
            if(ExtraRep!=0)
                lValue /= ExtraRep; 
            return IntValue.GetValue(); }

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
        template<MediumDecVariant VariantType=PartialDec>
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
        constexpr auto ConvertAsNormType = PartialDec::ConvertAsNormTypeV1<PartialDec>;

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
*/

protected:
		//Returns value as normal type representation
        template<MediumDecVariant VariantType=PartialDec>
        VariantType AutoConvertAsNormType()
        {
            VariantType Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormTypeV2 = PartialDec::AutoConvertAsNormType<PartialDec>;
*/
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=PartialDec>
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
		template<MediumDecVariant VariantType=PartialDec>
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
        constexpr auto BasicComparison = PartialDec::BasicComparisonV1<PartialDec>;
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = PartialDec::BasicComparisonWithoutSignCheck<PartialDec>;
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

		std::strong_ordering operator<=>(const PartialDec& that) const
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

		bool operator==(const PartialDec& that) const
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
        void PartialUIntDivOp(const IntType& rValue)
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
			DecimalHalf = DecimalRes;
    #else
			DecimalHalf.Value = DecimalRes;
    #endif
#endif
        }

        template<typename IntType=int>
        void PartialIntDivOp(const IntType& Value)
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

protected:
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicUIntDivOpV1(IntType& Value)
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
		
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicIntDivOpV1(IntType& Value)
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
/*
        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<PartialDec, IntType>;
		
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<PartialDec, IntType>;

		virtual void BasicDivOp(signed int& Value) { BasicIntDivOp(Value); }
		virtual void BasicDivOp(unsigned int& Value) { BasicUnsignedIntDivOp(Value); }
		virtual void BasicDivOp(signed long long& Value) { BasicIntDivOp(Value); }
        virtual void BasicDivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }
*/
    #pragma endregion NormalRep Integer Division Operations
		
    #pragma region NormalRep Integer Multiplication Operations
protected:
        template<typename IntType=int>
        void PartialUIntMultOp(const IntType& Value)
        {
			//Update this code
		}
		
        template<typename IntType=int>
        void PartialIntMultOp(const IntType& Value)
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
		
protected:
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
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
		
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
			PartialIntMultOp(Value);
            return *this;
        }
public:
/*
        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<PartialDec, const signed long long&>;
*/
		
    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:
	
public:
/*
        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntAddOp = BasicIntAddOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64AddOp = BasicUIntAddOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64AddOp = BasicIntAddOpV1<PartialDec, const signed long long&>;
*/
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<PartialDec, const signed long long&>;
*/
	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum Division Operations
protected:
	
public:
/*
        constexpr auto BasicDivOp = BasicDivOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Division Operations

	#pragma region NormalRep AltNum Multiplication Operations
protected:
		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned PartialDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicUnsignedMultOpV1(const PartialDec& rValue)
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
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= PartialDec::DecimalOverflowX;
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
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
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
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
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
                        IntValue = SelfIsNegative ? PartialDec::NegativeRep : 0;
                    }
				    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= PartialDec::DecimalOverflowX;
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
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
                        IntValue = SelfIsNegative ? PartialDec::NegativeRep : 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= PartialDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / PartialDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)PartialDec::DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? PartialDec::NegativeRep : 0;
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
public:
/*

        constexpr auto BasicMultOp = BasicMultOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
protected:
	
public:
/*
        constexpr auto BasicAddOp = BasicAddOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicSubOp = BasicSubOpV1<PartialDec>;
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
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, PartialDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator/=(PartialDec& self, PartialDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, PartialDec Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        friend PartialDec& operator*=(PartialDec& self, PartialDec Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, PartialDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator+=(PartialDec& self, PartialDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, PartialDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, PartialDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between PartialDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator+=(PartialDec& self, int Value) { return IntAddOp(self, Value); }

        //friend PartialDec operator+=(PartialDec* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator*=(PartialDec& self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator/=(PartialDec& self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// Addition Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between PartialDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator+=(PartialDec& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec operator*=(PartialDec& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/=(PartialDec& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend PartialDec operator+(PartialDec self, float Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, float Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, float Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, float Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(float Value, PartialDec self) { return (PartialDec)Value + self; }
        friend PartialDec operator-(float Value, PartialDec self) { return (PartialDec)Value - self; }
        friend PartialDec operator*(float Value, PartialDec self) { return (PartialDec)Value * self; }
        friend PartialDec operator/(float Value, PartialDec self) { return (PartialDec)Value / self; }

        friend PartialDec operator+(PartialDec self, double Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, double Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, double Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, double Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(PartialDec self, ldouble Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, ldouble Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, ldouble Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, ldouble Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(ldouble Value, PartialDec self) { return (PartialDec)Value + self; }
        friend PartialDec operator-(ldouble Value, PartialDec self) { return (PartialDec)Value - self; }
        friend PartialDec operator*(ldouble Value, PartialDec self) { return (PartialDec)Value * self; }
        friend PartialDec operator/(ldouble Value, PartialDec self) { return (PartialDec)Value / self; }


        friend PartialDec operator+(PartialDec self, unsigned char Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned char Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend PartialDec operator+(PartialDec self, unsigned short Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned short Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend PartialDec operator+(PartialDec self, unsigned int Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned int Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        

        friend PartialDec operator+=(PartialDec& self, unsigned int Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned int Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        
        friend PartialDec operator+(PartialDec self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-(PartialDec& self)
        {
            return self;//Negative numbers not supported
        }

        /// <summary>
        /// ++PartialDec Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator ++()
        {
            ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++PartialDec Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator --()
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
        /// <returns>PartialDec</returns>
        PartialDec operator ++(int)
        {
            PartialDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>PartialDec</returns>
        PartialDec operator --(int)
        {
            PartialDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator *()
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
        /// <returns>PartialDec</returns>
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
             *      - approximation of e^x in PartialDec precision
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
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions
	//Not Supported at moment
    #pragma endregion Trigonomic Etc Functions
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