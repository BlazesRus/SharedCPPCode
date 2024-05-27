// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"
#include "AltNumBase.h"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>
#include "..\..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\AltNumModResult.hpp"
#include "..\IntegerConcept.hpp"
#include "..\MediumDecVariantConcept.hpp"


#include "..\AlternativeInt\MirroredInt.hpp"
#include "..\AlternativeInt\PartialInt.hpp"
#include "..\RepType.h"

//"Not used for this variant" comment used as placeholder
// between unused regions to help with code compare between variants and keep structure similar
//Use  template<MediumDecVariant VariantType=MediumDec>
//to template functions for reuse with VariantTypes

namespace BlazesRusCode
{
    class MediumDec;
	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;
	
	// Forward declare VTable instances.
	//extern AltNum_VTable MediumDec_vtable;

/*---Accuracy Tests(with MediumDec based settings):
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication and division because of truncation
   (values get lost if get too small) (100% accuracy except for some truncated digits lost)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntHalf overflows/underflows
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like Pi(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API MediumDec
    {
protected:
#if defined(AltNum_UseBuiltinVirtualTable)
		template <typename T>
		void** GetVTable(T* obj) {
			return *((void***)obj);
		}
#endif
/*#if !defined(AltNum_DisableDefaultStringFormatOption)
        enum class DefaultStringFormatEnum : int
        {

        };
#endif*/
		//BitFlag 01(1) = PiRep
		RepTypeUnderlayer PiFlag = 1;
		//BitFlag 02(2) = ERep
		RepTypeUnderlayer EFlag = 2;
		//BitFlag 03(4) = IRep
		RepTypeUnderlayer IFlag = 4;
		//BitFlag 04(8) = Fractional Rep
		RepTypeUnderlayer FractionalFlag = 8;
		//If BitFlag#7 and BitFlag#4 is set and BitFlag#6 isn't set,
		//then Approaching From Top side;
		//If BitFlag#7 is set and BitFlag#6 isn't set,
		//then Approaching From Bottom side;
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
        static unsigned int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 1000000000;

        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
		
		//Maximum IntHalf that can be stored inside IntHalf field
        static MirroredInt const MaxIntHalf;
	
		//Minimum IntHalf that can be stored inside IntHalf field
        static MirroredInt const MinIntHalf;

	public:

        /// <summary>
        /// Value when IntHalf is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static MirroredInt const NegativeRep;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        MirroredInt IntHalf;

		//Return IntHalf as signed int
        signed int GetIntHalf()
        {
			return IntHalf.IsNegative()?((signed int)IntHalf.Value)*-1:(signed int)IntHalf.Value;
        }

        bool IsNegative()
        {
            return IntHalf.IsNegative();
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        PartialInt DecimalHalf;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDec(const MediumDec&) = default;

        MediumDec& operator=(const int& rhs)
        {
			if(rhs<0)
				IntHalf = MirroredInt(-rhs,0);
			else
				IntHalf = MirroredInt(rhs,1);
			DecimalHalf = 0;
            return *this;
        } const

        MediumDec& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const
        {
            return IntHalf.Value==0;
        }

        bool IsNotAtZeroInt() const
        {
            return IntHalf.Value!=0;
        }

        bool IsAtOneInt() const
        {
            return IntHalf.Value==1;
        }

        bool IsNotAtOneInt() const
        {
            return IntHalf.Value!=1;
        }

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero() const
		{
            return DecimalHalf==0&&IntHalf.Value==0;
		}
		
		bool IsOne() const
		{
            return DecimalHalf==0&&IntHalf==MirroredInt::One;
		}
		
		bool IsNegOne() const
		{
            return DecimalHalf==0&&IntHalf==MirroredInt::NegativeOne;
		}
		
		bool IsOneVal() const
		{
            return DecimalHalf==0&&IntHalf.Value==1;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDec>
        void SetVal(VariantType Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

		//Set value as exactly zero
        void SetAsZero()
        {
            IntHalf = 0;
            DecimalHalf = 0;
        }

		//Set value as exactly one
        void SetAsOne()
        {
            IntHalf = 1;
            DecimalHalf = 0;
        }
		
		//Set as +-1 while keeping current sign
        void SetAsOneVal()
        {
            IntHalf.Value = 1;
            DecimalHalf = 0;
        }
		
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            IntHalf.IsPositive ^= 1;
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
            IntHalf = MaxIntHalf;
			DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
            IntHalf = MinIntHalf;
			DecimalHalf = 999999999;
        }
	
    #pragma endregion RangeLimits

	#pragma region PiNum Setters
	//Not used for this variant(Used in MediumDecV2 and others)
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
	//Not used for this variant(Used in MediumDecV2 and others)
	#pragma endregion ENum Setters

    #pragma region INum Setters
    //Not used for this variant(Used in MediumDecV2 and others)
    #pragma endregion INum Setters

	#pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion Fractional Setters
        
	#pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion MixedFrac Setters
		
	#pragma region Infinity Setters
	//Not used for this variant(Used in MediumDecV2 and others)
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant(Used in MediumDecV2 and others)
	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntHalf = 0; DecimalHalf = NaNRep;
        }

        void SetAsUndefined()
        {
            IntHalf = 0; DecimalHalf = UndefinedRep;
        }
	#endif
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
				IntHalf.IsPositive = 0;
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
        template<MediumDecVariant VariantType=MediumDec>
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
        constexpr auto ConvertAsNormType = MediumDec::ConvertAsNormTypeV1<MediumDec>;

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
*/

protected:
		////Returns value as normal type representation
  //      template<MediumDecVariant VariantType=MediumDec>
  //      VariantType AutoConvertAsNormType()
  //      {
  //          VariantType Res = *this;
  //          Res.ConvertToNormTypeV2();
  //          return Res;
  //      }
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormTypeV2 = MediumDec::AutoConvertAsNormType<MediumDec>;
*/
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDec>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntHalf.IsPositive <=> that.IntHalf.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal; int rVal;
			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		template<MediumDecVariant VariantType=MediumDec>
		std::strong_ordering BasicComparisonWithoutSignCheck(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;

			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
	
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = MediumDec::BasicComparisonV1<MediumDec>;
		
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = MediumDec::BasicComparisonWithoutSignCheck<MediumDec>;
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntHalf.IsPositive <=> that.IntHalf.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal;
			if (auto IntHalfCmp = lVal <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			lVal = DecimalHalf.Value>0?1:0;
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		std::strong_ordering BasicIntComparisonV2(const int& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			int lVal = DecimalHalf.Value>0?1:0;
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
	
public:

		std::strong_ordering operator<=>(const MediumDec& that) const
		{
			return BasicComparison(that);
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

    #pragma region NormalRep Integer division operations
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
		
        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<UInt64>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<Int64>;

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto& BasicUIntDivOpV1(const IntType& Value)
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
            return *this;
        }
		
        template<IntegerType IntType=signed int>
        auto& BasicIntDivOpV1(const IntType& Value)
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
            return *this;
        }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicDivideByUIntV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicUIntDivOpV1(rValue);
        }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicDivideByIntV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicIntDivOpV1(rValue);
        }

public:

        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<unsigned int>;
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<const signed int>;
        constexpr auto UnsignedBasicIntDivOp = BasicUIntDivOpV1<signed int>;
        constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<UInt64>;
        constexpr auto BasicInt64DivOp = BasicIntDivOpV1<Int64>;
        constexpr auto UnsignedBasicInt64DivOp = BasicUIntDivOpV1<signed int>;

        constexpr auto BasicUInt8DivOp = BasicUIntDivOpV1<unsigned char>;
        constexpr auto BasicInt8DivOp = BasicIntDivOpV1<signed char>;
        constexpr auto BasicUInt16DivOp = BasicUIntDivOpV1<unsigned short>;
        constexpr auto BasicInt16DivOp = BasicIntDivOpV1<signed short>;

        constexpr auto BasicDivideByUInt = BasicDivideByUIntV1<unsigned int>;
        constexpr auto BasicDivideByInt = BasicDivideByIntV1<signed int>;
        constexpr auto UnsignedBasicDivideByInt = BasicDivideByUIntV1<signed int>;
        constexpr auto BasicDivideByUInt64 = BasicDivideByUIntV1<UInt64>;
        constexpr auto BasicDivideByInt64 = BasicDivideByIntV1<Int64>;
        constexpr auto UnsignedBasicDivideByInt64 = BasicDivideByUIntV1<signed int>;

        constexpr auto BasicDivideByUInt8 = BasicDivideByUIntV1<unsigned char>;
        constexpr auto BasicDivideByInt8 = BasicDivideByIntV1<signed char>;
        constexpr auto BasicDivideByUInt16 = BasicDivideByUIntV1<unsigned short>;
        constexpr auto BasicDivideByInt16 = BasicDivideByIntV1<signed short>;

    #pragma endregion NormalRep Integer division operations
		
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
                PartialUIntMultOp(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
public:
		
        constexpr auto PartialUIntMultOp = PartialUIntMultOpV1<unsigned int>;
        constexpr auto PartialIntMultOp = PartialIntMultOpV1<signed int>;
        constexpr auto UnsignedPartialIntMultOp = PartialUIntMultOpV1<signed int>;
        constexpr auto PartialUInt64MultOp = PartialUIntMultOpV1<UInt64>;
        constexpr auto PartialInt64MultOp = PartialIntMultOpV1<Int64>;
		
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

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicMultiplyByUIntV1(const IntType& rValue)
        {
        {
            auto self = *this;
            return self.BasicUIntMultOpV1(rValue);
        }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicMultiplyByIntV1(const IntType& rValue)
        {
        {
            auto self = *this;
            return self.BasicIntMultOpV1(rValue);
        }

public:

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<unsigned int>;
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<UInt64>;
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<Int64>;
        constexpr auto UnsignedBasicInt64MultOp = BasicUIntMultOpV1<Int64>;

        constexpr auto BasicUInt8MultOp = BasicUIntMultOpV1<unsigned char>;
        constexpr auto BasicInt8MultOp = BasicIntMultOpV1<signed char>;
        constexpr auto BasicUInt16MultOp = BasicUIntMultOpV1<unsigned short>;
        constexpr auto BasicInt16MultOp = BasicIntMultOpV1<signed short>;

        constexpr auto BasicMultiplyByUInt = BasicMultiplyByUIntV1<unsigned int>;
        constexpr auto BasicMultiplyByInt = BasicMultiplyByIntV1<signed int>;
        constexpr auto UnsignedBasicMultiplyByInt = BasicMultiplyByUIntV1<signed int>;
        constexpr auto BasicMultiplyByUInt64 = BasicMultiplyByUIntV1<UInt64>;
        constexpr auto BasicMultiplyByInt64 = BasicMultiplyByIntV1<Int64>;
        constexpr auto UnsignedBasicMultiplyByInt64 = BasicMultiplyByUIntV1<Int64>;

        constexpr auto BasicMultiplyByUInt8 = BasicMultiplyByUIntV1<unsigned char>;
        constexpr auto BasicMultiplyByInt8 = BasicMultiplyByIntV1<signed char>;
        constexpr auto BasicMultiplyByUInt16 = BasicMultiplyByUIntV1<unsigned short>;
        constexpr auto BasicMultiplyByInt16 = BasicMultiplyByIntV1<signed short>;
		
    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto& BasicIntAddOpV1(const IntType& rValue)
        {
			if(IntHalf.IsPositive())
			{
				if(rValue<0)
				{
					IntType invertedrValue = -rValue;
					//-2.XX + 3.XX = 0.XX
					if(invertedrValue>(IntType)IntHalf.Value)//Flips to other side of flag
					{
						IntHalf.IsPositive = 0;
						IntHalf.Value = rValue - IntHalf.Value + 1;
						if(DecimalHalf!=0)//Invert the decimal section
							DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
					}
					else
						IntHalf += rValue;
				}
				else
					IntHalf += rValue;
			}
			else
			{
				if(rValue<0)
					IntHalf.Value -= -rValue;
				else
				{
					//2.XX + -3.XX = -0.XX
					IntType invertedValue = -IntHalf.Value;
					if(invertedValue>=rValue)
						IntHalf -= -rValue;
					else//Flips to other side of flag
					{
						IntHalf.IsPositive = 1;
						IntHalf.Value = rValue - IntHalf.Value - 1;
						if(DecimalHalf!=0)//Invert the decimal section
							DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
					}
				}
			}
            return *this;
        }
		
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOpV1(const IntType& rValue)
        {
			if(IntHalf.IsPositive())
				IntHalf += rValue;
			else
			{
				IntType invertedValue = -IntHalf.Value;
				if(invertedValue>=rValue)
					IntHalf -= -rValue;
				else//Flips to other side of flag
				{
					IntHalf.IsPositive = 1;
					IntHalf.Value = rValue - IntHalf.Value - 1;
					if(DecimalHalf!=0)//Invert the decimal section
						DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
				}
			}
            return *this;
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicAddByUIntV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicUIntAddOpV1(rValue);
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicAddByIntV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicIntAddOpV1(rValue);
        }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicAddByUIntV1(const IntType& rValue)
        {
        {
            auto self = *this;
            return self.BasicAddByUIntV1(rValue);
        }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicAddByIntV1(const IntType& rValue)
        {
        {
            auto self = *this;
            return self.BasicAddByIntV1(rValue);
        }

public:

        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<unsigned int>;
        constexpr auto BasicIntAddOp = BasicIntAddOpV1<signed int>;
        constexpr auto UnsignedBasicIntAddOp = BasicUIntAddOpV1<signed int>;
        constexpr auto BasicUIntAddOp64 = BasicUIntAddOpV1<UInt64>;
        constexpr auto BasicIntAddOp64 = BasicIntAddOpV1<Int64>;
        constexpr auto UnsignedBasicIntAddOp64 = BasicUIntAddOpV1<signed int>;

        constexpr auto BasicUIntAddOp8 = BasicUIntAddOpV1<unsigned char>;
        constexpr auto BasicIntAddOp8 = BasicIntAddOpV1<signed char>;
        constexpr auto BasicUIntAddOp16 = BasicUIntAddOpV1<unsigned short>;
        constexpr auto BasicIntAddOp16 = BasicIntAddOpV1<signed short>;

        constexpr auto BasicAddByUInt = BasicAddByUIntV1<unsigned int>;
        constexpr auto BasicAddByInt = BasicAddByIntV1<signed int>;
        constexpr auto UnsignedBasicAddByInt = BasicAddByUIntV1<signed int>;
        constexpr auto BasicAddByUInt64 = BasicAddByUIntV1<UInt64>;
        constexpr auto BasicAddByInt64 = BasicAddByIntV1<Int64>;
        constexpr auto UnsignedBasicAddByInt64 = BasicAddByUIntV1<signed int>;

        constexpr auto BasicAddByUInt8 = BasicAddByUIntV1<unsigned char>;
        constexpr auto BasicAddByInt8 = BasicAddByIntV1<signed char>;
        constexpr auto BasicAddByUInt16 = BasicAddByUIntV1<unsigned short>;
        constexpr auto BasicAddByInt16 = BasicAddByIntV1<signed short>;

    	
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:

		/// <summary>
        /// Basic subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicIntSubOp(const IntType& rValue)
        {
                if(IntHalf.IsNegative())
                {
                    if(rValue<0)
                    {
						IntType invertedValue = -IntHalf.Value;
						IntType invertedrValue = -rValue;
						//-2.XX - -3.XX = 0.XX
						if(invertedrValue>invertedValue)//Flips to other side of flag
						{
							IntHalf.IsPositive = 1;
							IntHalf.Value = invertedrValue - IntHalf.Value - 1;
							if(DecimalHalf!=0)//Invert the decimal section
								DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
						}
						else
							IntHalf += -rValue;
                    }
                    else
                        IntHalf += -rValue;
                }
                else
                {
                    if(rValue<0)
                        IntHalf.Value -= rValue;
                    else
                    {
						//2.XX - 3.XX = -0.XX
						if(rValue>IntHalf.Value)//Flips to other side of flag
						{
							IntHalf.IsPositive = 0;
							IntHalf.Value = rValue - IntHalf.Value - 1;
							if(DecimalHalf!=0)//Invert the decimal section
								DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
						}
                    }
                }
            return *this;
        }
		
		/// <summary>
        /// Basic subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicUIntSubOp(const IntType& rValue)
        {
                if(IntHalf.IsNegative())
                {
                    if(rValue<0)
                    {
						IntType invertedValue = -IntHalf.Value;
						IntType invertedrValue = -rValue;
						//-2.XX - -3.XX = 0.XX
						if(invertedrValue>invertedValue)//Flips to other side of flag
						{
							IntHalf.IsPositive = 1;
							IntHalf.Value = invertedrValue - IntHalf.Value - 1;
							if(DecimalHalf!=0)//Invert the decimal section
								DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
						}
						else
							IntHalf += -rValue;
                    }
                    else
                        IntHalf += -rValue;
                }
                else
                {
                    if(rValue<0)
                        IntHalf.Value -= rValue;
                    else
                    {
						//2.XX - 3.XX = -0.XX
						if(rValue>IntHalf.Value)//Flips to other side of flag
						{
							IntHalf.IsPositive = 0;
							IntHalf.Value = rValue - IntHalf.Value - 1;
							if(DecimalHalf!=0)//Invert the decimal section
								DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
						}
                    }
                }
            return *this;
        }

		/// <summary>
        /// Basic subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicUIntSubV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicUIntSubOpV1(rValue);
        }

		/// <summary>
        /// Basic subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicIntSubV1(const IntType& rValue)
        {
            auto self = *this;
            return self.BasicIntSubOpV1(rValue);
        }

public:

        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<unsigned int>;
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<signed int>;
        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<UInt64>;
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<Int64>;

        constexpr auto BasicUIntSub = BasicUIntSubOpV1<unsigned int>;
        constexpr auto BasicIntSub = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSub = BasicUIntSubOpV1<signed int>;
        constexpr auto BasicUInt64Sub = BasicUIntSubOpV1<UInt64>;
        constexpr auto BasicInt64Sub = BasicIntSubOpV1<Int64>;


	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum division operations
protected:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const auto& rValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + (unsigned _int64)DecimalHalf;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf.Value + (unsigned _int64)rValue.DecimalHalf;	

            unsigned _int64 IntHalfRes = SelfRes / ValueRes;
            unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntHalf.Value = (unsigned int) IntHalfRes;
            DecimalHalf.Value = DecimalRes;		
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
		
public:

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedDivOp(const auto& rValue)
		{
			if(DecimalHalf==0)
			{
				if(rValue.DecimalHalf==0)
			        UnsignedBasicIntDivOp(rValue.IntHalf.Value);
                else if (UnsignedPartialDivOp(Value))//Prevent Dividing into nothing
				    DecimalHalf = 1;
			}
			else if (UnsignedPartialDivOp(Value))
			    DecimalHalf = 1;
            return *this;
		}

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void BasicDivOp(const auto& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                BasicUnsignedDivOp(-Value);
            }
            else
                BasicUnsignedDivOp(Value);
        }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicUnsignedDivision(const auto& rValue)
        {
            auto self = *this;
            return self.BasicUnsignedDivOp(rValue);
        }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicDivision(const auto& rValue)
        {
            auto self = *this;
            return self.BasicDivOp(rValue);
        }

	#pragma endregion NormalRep AltNum division operations

	#pragma region NormalRep AltNum Multiplication Operations
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedMultOp(const auto& rValue)
		{//To-Do:Update this code more towards current default format
            if (DecimalHalf == 0)
            {
                if (IntHalf == 1)
                {
                    IntHalf = rValue.IntHalf; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntHalf *= rValue.IntHalf;
                }
                else
                {
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;
                    bool ResIsNegative = IntHalf<0;
                    if(ResIsNegative)
                    {
                        ResIsNegative = false;
                        rRep *= IntHalfAsAbs();
                    }
                    else
                    {
                        ResIsNegative = true;
                        rRep *= IntHalf;
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntHalf = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
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
                        IntHalf = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntHalf.Value == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= MediumDec::DecimalOverflowX;
                if (rValue.IntHalf == 0)
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
                    SRep += (__int64)DecimalHalf * rValue.IntHalf;
                    if (SRep >= MediumDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDec::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDec::DecimalOverflowX;
                        IntHalf = OverflowVal;
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
                    __int64 SRep = MediumDec::DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.IntHalf;
                    if (SRep >= MediumDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDec::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDec::DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
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
                        IntHalf.Value = 0;
                    }
				    return *this;
                }
                else if (rValue.IntHalf == 0)
                {
                    __int64 SRep = MediumDec::DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= MediumDec::DecimalOverflowX;
                    if (SRep >= MediumDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDec::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDec::DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
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
                        IntHalf.Value = 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntHalf == 0 ? DecimalHalf : MediumDec::DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= MediumDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDec::DecimalOverflow;
                    IntHalf.Value = (unsigned int) IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntHalf==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

        void BasicMultOp(const auto& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                BasicUnsignedMultOp(-Value);
            }
            else
                BasicUnsignedMultOp(Value);
        }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicUnsignedMultiplication(const auto& rValue)
        {
            auto self = *this;
            return self.BasicUnsignedDivOp(rValue);
        }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicMultiplication(const auto& rValue)
        {
            auto self = *this;
            return self.BasicMultOp(rValue);
        }

/*
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultiplication = MediumDec::BasicMultiplication;
		
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedMultiplication = MediumDec::BasicUnsignedMultiplication;
*/

	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
protected:
	
public:
/*
        constexpr auto BasicAddOp = BasicAddOpV1<MediumDec>;
*/
	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicSubOp = BasicSubOpV1<MediumDec>;
*/
	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Mixed Fraction Operations
	//Used only in AltDec/MixedDec
	#pragma endregion Mixed Fraction Operations

	#pragma region Other division operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        void DivideByTwo()
        {
            if(DecimalHalf==0&&IntHalf&1==1)//Check if number is odd
                rValue.UnsignedBasicIntDivOp(2);
            else
                IntHalf /= 2;
        }

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        void DivideByFour()
        {
            //Checking if divisible by 4 based on
            //https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
            //checking if divible by 8 equals (((n >> 3) << 3) == n)
            if(DecimalHalf==0&&(((IntHalf >> 2) << 2) == IntHalf))//Check if number can be perfectly divided by 4
                IntHalf /= 4;
            else
                rValue.UnsignedBasicIntDivOp(4);
        }
	
protected:
        /// <summary>
        /// Division operation between MediumDec and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntDivOpV1 = BasicUIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntDivOpV1 = BasicIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivideByUIntV1 = BasicDivideByUIntV1<IntType>;

        /// <summary>
        /// Division operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto DivideByIntV1 = BasicDivideByIntV1<IntType>;

public:

        constexpr auto UIntDivOpV1 = BasicUIntDivOpV1<unsigned int>;
        constexpr auto IntDivOpV1 = BasicIntDivOpV1<signed int>;
        constexpr auto UnsignedBasicIntDivOp = UnsignedBasicIntMultOp<signed int>;
        constexpr auto UInt64DivOp = BasicUIntDivOpV1<UInt64>;
        constexpr auto Int64DivOp = BasicIntDivOpV1<Int64>;
	
        constexpr auto DivideByUInt = BasicDivideByUInt;
        constexpr auto DivideByInt = BasicDivideByInt;
        constexpr auto UnsignedDivideByInt = UnsignedBasicDivideByInt;
        constexpr auto DivideByUInt64 = BasicDivideByUInt64;
        constexpr auto DivideByInt64 = BasicDivideByInt64;
        constexpr auto UnsignedDivideByInt64 = UnsignedBasicDivideByInt64;
		
        constexpr auto DivideByUInt8 = BasicDivideByUInt8;
        constexpr auto DivideByInt8 = BasicDivideByInt8;
        constexpr auto DivideByUInt16 = BasicDivideByUInt16;
        constexpr auto DivideByInt16 = BasicDivideByInt16;

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& UnsignedDivOp(const auto& rValue)
		{
			if(DecimalHalf==0)
			{
				if(rValue.DecimalHalf==0)
				{
					switch(rValue.IntHalf.Value)
					{
						case 2:
							if(IntHalf&1==1)//Check if number is odd
								UnsignedBasicIntDivOp(2);
							else
								IntHalf.Value /= 2;
							break;
						case 4:
							if(((IntHalf >> 2) << 2) == IntHalf)
								IntHalf.Value /= 4;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 8:
							if(((IntHalf >> 3) << 3) == IntHalf)
								IntHalf.Value /= 8;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 16:
							if(((IntHalf >> 4) << 4) == IntHalf)
								IntHalf.Value /= 16;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 32:
							if(((IntHalf >> 5) << 5) == IntHalf)
								IntHalf.Value /= 32;
							else
								UnsignedBasicIntDivOp(4);
							break;
                        case 0:
                            throw "Target value can not be divided by zero";
                            break;
						default:
							UnsignedBasicIntDivOp(rValue.IntHalf.Value);
							break;
					}
				}
				else if (UnsignedPartialDivOp(Value))//Prevent Dividing into nothing
				    DecimalHalf = 1;
			}
			else if (UnsignedPartialDivOp(Value))//Prevent Dividing into nothing
		        DecimalHalf = 1;
            return *this;
		}

		/// <summary>
        /// Division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void DivOp(const auto& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedDivOp(Value);
        }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto DivideByUnsigned(const auto& rValue)
        {
            auto self = *this;
            return self.UnsignedDivOp(rValue);
        }

		/// <summary>
        /// Division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto DivideBy(const auto& rValue)
        {
            auto self = *this;
            return self.DivOp(rValue);
        }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const MediumDec& Value) { return self.DivideBy(Value); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const MediumDec& Value) { return self.DivOp(Value); }
		
        /// <summary>
        /// Division operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const signed int& Value) { return self.DivideByInt(Value); }
        friend MediumDec operator/(const MediumDec& self, const Int64& Value) { return self.DivideByInt64(Value); }
        friend MediumDec operator/(const MediumDec& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend MediumDec operator/(const MediumDec& self, const UInt64& Value) { return self.DivideByUInt64(Value); }
		
        friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const Int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const UInt64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

        friend MediumDec operator/(const MediumDec& self, const signed char& Value) { return self.DivideByInt8(Value); }
        friend MediumDec operator/(const MediumDec& self, const signed short& Value) { return self.DivideByInt16(Value); }
        friend MediumDec operator/(const MediumDec& self, const unsigned char& Value) { return self.DivideByUInt8(Value); }
        friend MediumDec operator/(const MediumDec& self, const unsigned short& Value) { return self.DivideByUInt16(Value); }

        friend MediumDec operator/(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const signed int& Value) { return self.IntDivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const Int64& Value) { return self.Int64DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const UInt64& Value) { return self.UInt64DivOp(Value); }

        friend MediumDec& operator/=(MediumDec& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }

	#pragma endregion Other division operations	

	#pragma region Other multiplication operations
		
        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        void MultipleByTwo()
        {
	        UnsignedBasicIntMultOp(2);
        }

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        void MultipleByFour()
        {
	        UnsignedBasicIntMultOp(4);
        }
		
//Replace with actual functions inside MediumDecV2 and other derived variants
protected:
        /// <summary>
        /// Multiplication operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntMultOpV1 = BasicIntMultOpV1<IntType>;

public:

        constexpr auto UIntMultOpV1 = BasicUIntMultOpV1<unsigned int>;
        constexpr auto IntMultOpV1 = BasicIntMultOpV1<signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto UInt64MultOp = BasicUIntMultOpV1<UInt64>;
        constexpr auto Int64MultOp = BasicIntMultOpV1<Int64>;
	
        constexpr auto MultipleByUInt = BasicMultipleByUInt;
        constexpr auto MultipleByInt = BasicMultipleByInt;
        constexpr auto UnsignedMultipleByInt = UnsignedBasicMultipleByInt;
        constexpr auto MultipleByUInt64 = BasicMultipleByUInt64;
        constexpr auto MultipleByInt64 = BasicMultipleByInt64;
        constexpr auto UnsignedMultipleByInt64 = UnsignedBasicMultipleByInt64;
		
        constexpr auto MultipleByUInt8 = BasicMultipleByUInt8;
        constexpr auto MultipleByInt8 = BasicMultipleByInt8;
        constexpr auto MultipleByUInt16 = BasicMultipleByUInt16;
        constexpr auto MultipleByInt16 = BasicMultipleByInt16;

protected:


public:

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto UnsignedMultOp = MediumDec::BasicUnsignedMultOp;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto MultOp = MediumDec::BasicMultOp;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto MultipleByUnsigned = MediumDec::BasicUnsignedMultiplication;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto MultipleBy = MediumDec::BasicMultiplication;

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& self, const MediumDec& Value) { return self.MultipleBy(Value); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& self, const MediumDec& Value) { return self.MultOp(Value); }
		
        /// <summary>
        /// Multiplication operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& self, const signed int& Value) { return self.MultiplyByInt(Value); }
        friend MediumDec operator*(const MediumDec& self, const Int64& Value) { return self.MultiplyByInt64(Value); }
        friend MediumDec operator*(const MediumDec& self, const unsigned int& Value) { return self.MultiplyByUInt(Value); }
        friend MediumDec operator*(const MediumDec& self, const UInt64& Value) { return self.MultiplyByUInt64(Value); }
		
        friend MediumDec operator*(const signed int& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt(lValue); }
        friend MediumDec operator*(const Int64& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt64(lValue); }
        friend MediumDec operator*(const unsigned int& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt(lValue); }
        friend MediumDec operator*(const UInt64& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt64(lValue); }

        friend MediumDec operator*(const MediumDec& self, const signed char& Value) { return self.MultiplyByInt8(Value); }
        friend MediumDec operator*(const MediumDec& self, const signed short& Value) { return self.MultiplyByInt16(Value); }
        friend MediumDec operator*(const MediumDec& self, const unsigned char& Value) { return self.MultiplyByUInt8(Value); }
        friend MediumDec operator*(const MediumDec& self, const unsigned short& Value) { return self.MultiplyByUInt16(Value); }

        friend MediumDec operator*(const signed char& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt8(lValue); }
        friend MediumDec operator*(const signed short& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt16(lValue); }
        friend MediumDec operator*(const unsigned char& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt8(lValue); }
        friend MediumDec operator*(const unsigned short& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt16(lValue); }

        /// <summary>
        /// *= operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& self, const signed int& Value) { return self.IntMultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const Int64& Value) { return self.Int64MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const UInt64& Value) { return self.UInt64MultOp(Value); }

        friend MediumDec& operator*=(MediumDec& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

	#pragma endregion Other multiplication operations

	#pragma region Other addition operations
protected:

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntAddOpV1 = BasicUIntAddOpV1<IntType>;

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntAddOpV1 = BasicIntAddOpV1<IntType>;

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto AddByUIntV1 = BasicAddByUIntV1<IntType>;

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType= signed int>
        constexpr auto AddByIntV1 = BasicAddByIntV1<IntType>;

public:

        constexpr auto UIntAddOpV1 = BasicUIntAddOpV1;
        constexpr auto IntAddOpV1 = BasicIntAddOp;
        constexpr auto UnsignedIntAddOp = UnsignedBasicIntAddOp;
        constexpr auto UInt64AddOp = BasicUInt64AddOp;
        constexpr auto Int64AddOp = BasicInt64AddOp;
	
        constexpr auto AddByUInt = BasicAddByUInt;
        constexpr auto AddByInt = BasicAddByInt;
        constexpr auto UnsignedAddByInt = UnsignedBasicAddByInt;
        constexpr auto AddByUInt64 = BasicAddByUInt64;
        constexpr auto AddByInt64 = BasicAddByInt64;
        constexpr auto UnsignedAddByInt64 = UnsignedBasicAddByInt64;
		
        constexpr auto AddByUInt8 = BasicAddByUInt8;
        constexpr auto AddByInt8 = BasicAddByInt8;
        constexpr auto AddByUInt16 = BasicAddByUInt16;
        constexpr auto AddByInt16 = BasicAddByInt16;

        /// <summary>
        /// Unsigned Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto UnsignedAddOp = MediumDec::BasicUnsignedAddOp;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto AddOp = MediumDec::BasicAddOp;

        /// <summary>
        /// Unsigned Addition operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto AddByUnsigned = MediumDec::BasicUnsignedMultiplication;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto AddBy = MediumDec::BasicMultiplication;

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& self, const MediumDec& Value) { return self.AddBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& self, const MediumDec& Value) { return self.AddOp(Value); }
		
        /// <summary>
        /// Addition operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& self, const signed int& Value) { return self.AddByInt(Value); }
        friend MediumDec operator+(const MediumDec& self, const Int64& Value) { return self.AddByInt64(Value); }
        friend MediumDec operator+(const MediumDec& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend MediumDec operator+(const MediumDec& self, const UInt64& Value) { return self.AddByUInt64(Value); }
		
        friend MediumDec operator+(const signed int& lValue, const MediumDec& rValue) { return rValue.AddByInt(lValue); }
        friend MediumDec operator+(const Int64& lValue, const MediumDec& rValue) { return rValue.AddByInt64(lValue); }
        friend MediumDec operator+(const unsigned int& lValue, const MediumDec& rValue) { return rValue.AddByUInt(lValue); }
        friend MediumDec operator+(const UInt64& lValue, const MediumDec& rValue) { return rValue.AddByUInt64(lValue); }

        friend MediumDec operator+(const MediumDec& self, const signed char& Value) { return self.AddByInt8(Value); }
        friend MediumDec operator+(const MediumDec& self, const signed short& Value) { return self.AddByInt16(Value); }
        friend MediumDec operator+(const MediumDec& self, const unsigned char& Value) { return self.AddByUInt8(Value); }
        friend MediumDec operator+(const MediumDec& self, const unsigned short& Value) { return self.AddByUInt16(Value); }

        friend MediumDec operator+(const signed char& lValue, const MediumDec& rValue) { return rValue.AddByInt8(lValue); }
        friend MediumDec operator+(const signed short& lValue, const MediumDec& rValue) { return rValue.AddByInt16(lValue); }
        friend MediumDec operator+(const unsigned char& lValue, const MediumDec& rValue) { return rValue.AddByUInt8(lValue); }
        friend MediumDec operator+(const unsigned short& lValue, const MediumDec& rValue) { return rValue.AddByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& self, const signed int& Value) { return self.IntAddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const Int64& Value) { return self.Int64AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const UInt64& Value) { return self.UInt64AddOp(Value); }

        friend MediumDec& operator+=(MediumDec& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

	#pragma endregion Other addition operations

	#pragma region Other Subtraction Operations
protected:

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto& UIntSubOpV1 = BasicUIntSubOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntSubOpV1 = BasicIntSubOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto SubByUIntV1 = BasicSubtractByUIntV1;


        /// <summary>
        /// Subtraction operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase</returns>
        template<IntegerType IntType= signed int>
        constexpr auto SubByIntV1 = BasicSubtractByIntV1;

public:

        constexpr auto UIntSubOpV1 = BasicUIntSubOpV1<unsigned int>;
        constexpr auto IntSubOpV1 = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSubOp = UnsignedBasicIntMultOp<signed int>;
        constexpr auto UInt64SubOp = BasicUIntSubOpV1<UInt64>;
        constexpr auto Int64SubOp = BasicIntSubOpV1<Int64>;
	
        constexpr auto SubtractByUInt = BasicSubtractByUInt;
        constexpr auto SubtractByInt = BasicSubtractByInt;
        constexpr auto UnsignedSubtractByInt = UnsignedBasicSubtractByInt;
        constexpr auto SubtractByUInt64 = BasicSubtractByUInt64;
        constexpr auto SubtractByInt64 = BasicSubtractByInt64;
        constexpr auto UnsignedSubtractByInt64 = UnsignedBasicSubtractByInt64;
		
        constexpr auto SubtractByUInt8 = BasicSubtractByUInt8;
        constexpr auto SubtractByInt8 = BasicSubtractByInt8;
        constexpr auto SubtractByUInt16 = BasicSubtractByUInt16;
        constexpr auto SubtractByInt16 = BasicSubtractByInt16;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto UnsignedSubOp = MediumDec::BasicUnsignedSubOp;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto SubOp = MediumDec::BasicSubOp;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto SubtractByUnsigned = MediumDec::BasicUnsignedDivision;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto SubtractBy = MediumDec::BasicDivision;

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& self, const MediumDec& Value) { return self.SubtractBy(Value); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, const MediumDec& Value) { return self.SubOp(Value); }
		
        /// <summary>
        /// Subtraction operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& self, const signed int& Value) { return self.SubtractByInt(Value); }
        friend MediumDec operator-(const MediumDec& self, const Int64& Value) { return self.SubtractByInt64(Value); }
        friend MediumDec operator-(const MediumDec& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend MediumDec operator-(const MediumDec& self, const UInt64& Value) { return self.SubtractByUInt64(Value); }
		
        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const Int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const UInt64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }

        friend MediumDec operator-(const MediumDec& self, const signed char& Value) { return self.SubtractByInt8(Value); }
        friend MediumDec operator-(const MediumDec& self, const signed short& Value) { return self.SubtractByInt16(Value); }
        friend MediumDec operator-(const MediumDec& self, const unsigned char& Value) { return self.SubtractByUInt8(Value); }
        friend MediumDec operator-(const MediumDec& self, const unsigned short& Value) { return self.SubtractByUInt16(Value); }

        friend MediumDec operator-(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }


        /// <summary>
        /// -= operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& self, const signed int& Value) { return self.IntSubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const Int64& Value) { return self.Int64SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const UInt64& Value) { return self.UInt64SubOp(Value); }

        friend MediumDec& operator-=(MediumDec& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

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
                IntHalf.IsPositive = 1;
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
                    IntHalf.IsPositive = 1;
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
                    IntHalf.IsPositive = 0;
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
                        IsPositive = 1;
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
                        IntHalf.IsPositive = 0;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.IsPositive = 1;
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
                    IntHalf.IsPositive = 0;
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
				IntHalf.IsPositive = 0;
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
				IntHalf.IsPositive = 0;
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