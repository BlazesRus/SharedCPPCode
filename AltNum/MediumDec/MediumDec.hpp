// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#if defined(AltNum_UseOldDivisionCode)
	#include <boost/multiprecision/cpp_int.hpp>
#endif

#include "AltNumModResult.hpp"

#include "..\MediumDec\MediumDecBase.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/
/*
  Using
  const auto& new_fn_name = old_fn_name;
  
  or
  
  constexpr auto Function = MediumDecBase::Function<templateTypename>;
  
  or
  
  template<typename T>
constexpr auto alias_to_old = old_function<T>;
  
  to reference functions from base class
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

		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModChecker<MediumDec>{};
		
	#pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const IntHalfType& intVal = 0, const DecimalHalfType& decVal = 0)
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

	#pragma endregion class_constructors

	#pragma region Check_if_Zero

        //Is at either zero or negative zero IntHalf of AltNum
        constexpr auto IsAtZeroInt = MediumDecBase::IsAtZeroInt;

        //alias function
        constexpr auto IsNotAtZeroInt = MediumDecBase::IsNotAtZeroInt;

        //Detect if at exactly zero
        constexpr auto IsZero = MediumDecBase::IsZero;

        constexpr auto SetAsZero = MediumDecBase::SetAsZero;

	#pragma endregion Check_if_Zero

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The right side value.</param>
        void SetVal(MediumDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;


    #pragma region Const Representation values
    //Variables for this section stored inside derivable base class
    #pragma endregion Const Representation values

    #pragma region RepType

/*
        constexpr auto RepTypeAsString = MediumDecBase::RepTypeAsString;

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        constexpr auto GetRepType = MediumDecBase::GetRepType;
*/
    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMaximum = MediumDecBase::SetAsMaximum;

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMinimum = MediumDecBase::SetAsMinimum;

    #pragma endregion RangeLimits

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
#if !defined(AltNum_EnableMirroredSection)
            MediumDec NewSelf = MediumDec(-1);
#else
            MediumDec NewSelf = MediumDec(MirroredInt(1,0));
#endif
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
            return MediumDec(MinIntValue, 999999999);
        }

        static MediumDec MaximumValue()
        {
            return MediumDec(MaxIntValue, 999999999);
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
        /// <param name="Value">The right side value.</param>
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
        /// <param name="Value">The right side value.</param>
        MediumDec(float Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The right side value.</param>
        MediumDec(double Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The right side value.</param>
        MediumDec(ldouble Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The right side value.</param>
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
    //Not using in MediumDec
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

	#pragma region Division Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOpV1 = MediumDecBase::PartialUIntDivOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOpV1 = MediumDecBase::PartialIntDivOpV1<IntType>;

        /// <summary>
        /// Basic Division operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOpV1 = MediumDecBase::BasicUIntDivOpV1<IntType>;

        /// <summary>
        /// Basic Division operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOpV1 = MediumDecBase::BasicIntDivOpV1<IntType>;

        /// <summary>
        /// Basic Division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicDivideByUIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;

        /// <summary>
        /// Basic Division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicDivideByIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<unsigned long long>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<signed long long>;

        constexpr auto BasicDivideByUInt = BasicDivideByUIntV1<unsigned int>;
        constexpr auto BasicDivideByInt = BasicDivideByIntV1<signed int>;
        constexpr auto UnsignedBasicDivideByInt = BasicDivideByUIntV1<signed int>;
        constexpr auto BasicDivideByUInt64 = BasicDivideByUIntV1<unsigned long long>;
        constexpr auto BasicDivideByInt64 = BasicDivideByIntV1<signed long long>;
        constexpr auto UnsignedBasicDivideByInt64 = BasicDivideByUIntV1<signed int>;

        constexpr auto BasicDivideByUInt8 = BasicDivideByUIntV1<unsigned char>;
        constexpr auto BasicDivideByInt8 = BasicDivideByIntV1<signed char>;
        constexpr auto BasicDivideByUInt16 = BasicDivideByUIntV1<unsigned short>;
        constexpr auto BasicDivideByInt16 = BasicDivideByIntV1<signed short>;
		
        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideByTwo = MediumDecBase::DivideByTwo;

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideByFour = MediumDecBase::DivideByFour;
	
//Replace with actual functions inside MediumDecV2 and other derived variants	
protected:
        /// <summary>
        /// Division operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntDivOpV1 = BasicIntDivOpV1<IntType>;

public:

        constexpr auto UIntDivOpV1 = BasicUIntDivOpV1<unsigned int>;
        constexpr auto IntDivOpV1 = BasicIntDivOpV1<signed int>;
        constexpr auto UnsignedBasicIntDivOp = UnsignedBasicIntMultOp<signed int>;
        constexpr auto UInt64DivOp = BasicUIntDivOpV1<unsigned long long>;
        constexpr auto Int64DivOp = BasicIntDivOpV1<signed long long>;
	
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

protected:

        constexpr auto PartialDivOp = MediumDecBase::PartialDivOp;
        constexpr auto UnsignedPartialDivOp = MediumDecBase::UnsignedPartialDivOp;

public:
	
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedDivOp = MediumDecBase::BasicUnsignedDivOp;

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivOp = MediumDecBase::BasicDivOp;
    
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = MediumDecBase::BasicUnsignedDivision;

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = MediumDecBase::BasicDivision;

        /// <summary>
        /// Division operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto UnsignedDivOp = MediumDecBase::BasicUnsignedDivOp;

        /// <summary>
        /// Division operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivOp = MediumDecBase::BasicDivOp;

        /// <summary>
        /// Division operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideByUnsigned = MediumDecBase::BasicUnsignedDivision;

        /// <summary>
        /// Division operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideBy = MediumDecBase::BasicDivision;

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
        friend MediumDec operator/(const MediumDec& self, const signed long long& Value) { return self.DivideByInt64(Value); }
        friend MediumDec operator/(const MediumDec& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend MediumDec operator/(const MediumDec& self, const unsigned long long& Value) { return self.DivideByUInt64(Value); }
		
        friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

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
        friend MediumDec& operator/=(MediumDec& self, const signed long long& Value) { return self.Int64DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned long long& Value) { return self.UInt64DivOp(Value); }

        friend MediumDec& operator/=(MediumDec& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }

	#pragma endregion Division Operations

	#pragma region Multiplication Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntMultOpV1 = MediumDecBase::PartialUIntMultOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOpV1 = MediumDecBase::PartialIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntMultOpV1 = MediumDecBase::BasicUIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOpV1 = MediumDecBase::BasicIntMultOpV1<IntType>;
	
        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicMultipleByUIntV1 = MediumDecBase::BasicMultipleByUIntV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicMultipleByIntV1 = MediumDecBase::BasicMultipleByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntMultOpV1 = PartialUIntMultOpV1<unsigned int>;
        constexpr auto PartialIntMultOpV1 = PartialIntMultOpV1<signed int>;
        constexpr auto UnsignedPartialIntMultOpV1 = PartialUIntMultOpV1<signed int>;
        constexpr auto PartialUInt64MultOpV1 = PartialUIntMultOpV1<unsigned long long>;
        constexpr auto PartialInt64MultOpV1 = PartialIntMultOpV1<signed long long>;

        constexpr auto BasicMultipleByUInt = BasicMultipleByUIntV1<unsigned int>;
        constexpr auto BasicMultipleByInt = BasicMultipleByIntV1<signed int>;
        constexpr auto UnsignedBasicMultipleByInt = BasicMultipleByUIntV1<signed int>;
        constexpr auto BasicMultipleByUInt64 = BasicMultipleByUIntV1<unsigned long long>;
        constexpr auto BasicMultipleByInt64 = BasicMultipleByIntV1<signed long long>;
        constexpr auto UnsignedBasicMultipleByInt64 = BasicMultipleByUIntV1<signed int>;

        constexpr auto BasicMultipleByUInt8 = BasicMultipleByUIntV1<unsigned char>;
        constexpr auto BasicMultipleByInt8 = BasicMultipleByIntV1<signed char>;
        constexpr auto BasicMultipleByUInt16 = BasicMultipleByUIntV1<unsigned short>;
        constexpr auto BasicMultipleByInt16 = BasicMultipleByIntV1<signed short>;
		
        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleByTwo = MediumDecBase::MultipleByTwo;

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleByFour = MediumDecBase::MultipleByFour;
		
//Replace with actual functions inside MediumDecV2 and other derived variants
protected:
        /// <summary>
        /// Multiplication operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntMultOpV1 = BasicIntMultOpV1<IntType>;

public:

        constexpr auto UIntMultOpV1 = BasicUIntMultOpV1<unsigned int>;
        constexpr auto IntMultOpV1 = BasicIntMultOpV1<signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto UInt64MultOp = BasicUIntMultOpV1<unsigned long long>;
        constexpr auto Int64MultOp = BasicIntMultOpV1<signed long long>;
	
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

        constexpr auto PartialMultOp = MediumDecBase::PartialMultOp;
        constexpr auto UnsignedPartialMultOp = MediumDecBase::UnsignedPartialMultOp;

public:
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedMultOp = MediumDecBase::BasicUnsignedMultOp;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultOp = MediumDecBase::BasicMultOp;

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedMultiplication = MediumDecBase::BasicUnsignedMultiplication;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultiplication = MediumDecBase::BasicMultiplication;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto UnsignedMultOp = MediumDecBase::BasicUnsignedMultOp;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultOp = MediumDecBase::BasicMultOp;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleByUnsigned = MediumDecBase::BasicUnsignedMultiplication;

        /// <summary>
        /// Multiplication operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleBy = MediumDecBase::BasicMultiplication;

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
        friend MediumDec operator*(const MediumDec& self, const signed long long& Value) { return self.MultiplyByInt64(Value); }
        friend MediumDec operator*(const MediumDec& self, const unsigned int& Value) { return self.MultiplyByUInt(Value); }
        friend MediumDec operator*(const MediumDec& self, const unsigned long long& Value) { return self.MultiplyByUInt64(Value); }
		
        friend MediumDec operator*(const signed int& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt(lValue); }
        friend MediumDec operator*(const signed long long& lValue, const MediumDec& rValue) { return rValue.MultiplyByInt64(lValue); }
        friend MediumDec operator*(const unsigned int& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt(lValue); }
        friend MediumDec operator*(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.MultiplyByUInt64(lValue); }

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
        friend MediumDec& operator*=(MediumDec& self, const signed long long& Value) { return self.Int64MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned long long& Value) { return self.UInt64MultOp(Value); }

        friend MediumDec& operator*=(MediumDec& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend MediumDec& operator*=(MediumDec& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

	#pragma endregion Multiplication Operations

	#pragma region Addition Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntAddOpV1 = MediumDecBase::PartialUIntAddOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntAddOpV1 = MediumDecBase::PartialIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntAddOpV1 = MediumDecBase::BasicUIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOpV1 = MediumDecBase::BasicIntAddOpV1<IntType>;
	
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicAddByUIntV1 = MediumDecBase::BasicAddByUIntV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicAddByIntV1 = MediumDecBase::BasicAddByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntAddOpV1 = PartialUIntAddOpV1<unsigned int>;
        constexpr auto PartialIntAddOpV1 = PartialIntAddOpV1<signed int>;
        constexpr auto UnsignedPartialIntAddOpV1 = PartialUIntAddOpV1<signed int>;
        constexpr auto PartialUInt64AddOpV1 = PartialUIntAddOpV1<unsigned long long>;
        constexpr auto PartialInt64AddOpV1 = PartialIntAddOpV1<signed long long>;

        constexpr auto BasicAddByUInt = BasicAddByUIntV1<unsigned int>;
        constexpr auto BasicAddByInt = BasicAddByIntV1<signed int>;
        constexpr auto UnsignedBasicAddByInt = BasicAddByUIntV1<signed int>;
        constexpr auto BasicAddByUInt64 = BasicAddByUIntV1<unsigned long long>;
        constexpr auto BasicAddByInt64 = BasicAddByIntV1<signed long long>;
        constexpr auto UnsignedBasicAddByInt64 = BasicAddByUIntV1<signed int>;

        constexpr auto BasicAddByUInt8 = BasicAddByUIntV1<unsigned char>;
        constexpr auto BasicAddByInt8 = BasicAddByIntV1<signed char>;
        constexpr auto BasicAddByUInt16 = BasicAddByUIntV1<unsigned short>;
        constexpr auto BasicAddByInt16 = BasicAddByIntV1<signed short>;
		
//Replace with actual functions inside MediumDecV2 and other derived variants
protected:
        /// <summary>
        /// Addition operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntAddOpV1 = BasicIntAddOpV1<IntType>;

public:

        constexpr auto UIntAddOpV1 = BasicUIntAddOpV1<unsigned int>;
        constexpr auto IntAddOpV1 = BasicIntAddOpV1<signed int>;
        constexpr auto UnsignedBasicIntAddOp = BasicUIntAddOpV1<signed int>;
        constexpr auto UInt64AddOp = BasicUIntAddOpV1<unsigned long long>;
        constexpr auto Int64AddOp = BasicIntAddOpV1<signed long long>;
	
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

protected:

        constexpr auto PartialAddOp = MediumDecBase::PartialAddOp;
        constexpr auto UnsignedPartialAddOp = MediumDecBase::UnsignedPartialAddOp;

public:
		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned MediumDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedAddOp = MediumDecBase::BasicUnsignedAddOp;

		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned MediumDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddOp = MediumDecBase::BasicAddOp;

		/// <summary>
        /// Basic unsigned addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedAddition = MediumDecBase::BasicUnsignedAddition;

		/// <summary>
        /// Basic addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddition = MediumDecBase::BasicAddition;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto UnsignedAddOp = MediumDecBase::BasicUnsignedAddOp;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto AddOp = MediumDecBase::BasicAddOp;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto AddByUnsigned = MediumDecBase::BasicUnsignedAddition;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto AddBy = MediumDecBase::BasicAddition;

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
        friend MediumDec operator+(const MediumDec& self, const signed long long& Value) { return self.AddByInt64(Value); }
        friend MediumDec operator+(const MediumDec& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend MediumDec operator+(const MediumDec& self, const unsigned long long& Value) { return self.AddByUInt64(Value); }
		
        friend MediumDec operator+(const signed int& lValue, const MediumDec& rValue) { return rValue.AddByInt(lValue); }
        friend MediumDec operator+(const signed long long& lValue, const MediumDec& rValue) { return rValue.AddByInt64(lValue); }
        friend MediumDec operator+(const unsigned int& lValue, const MediumDec& rValue) { return rValue.AddByUInt(lValue); }
        friend MediumDec operator+(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.AddByUInt64(lValue); }

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
        friend MediumDec& operator+=(MediumDec& self, const signed long long& Value) { return self.Int64AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned long long& Value) { return self.UInt64AddOp(Value); }

        friend MediumDec& operator+=(MediumDec& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend MediumDec& operator+=(MediumDec& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntSubOpV1 = MediumDec::PartialUIntSubOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntSubOpV1 = MediumDec::PartialIntSubOpV1<IntType>;

        /// <summary>
        /// Basic Subtraction operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntSubOpV1 = MediumDec::BasicUIntSubOpV1<IntType>;

        /// <summary>
        /// Basic Subtraction operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOpV1 = MediumDec::BasicIntSubOpV1<IntType>;

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicSubtractByUIntV1 = MediumDec::BasicSubtractByIntV1<IntType>;

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicSubtractByIntV1 = MediumDec::BasicSubtractByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntSubOpV1 = PartialUIntSubOpV1<unsigned int>;
        constexpr auto PartialIntSubOpV1 = PartialIntSubOpV1<signed int>;
        constexpr auto UnsignedPartialIntSubOpV1 = PartialUIntSubOpV1<signed int>;
        constexpr auto PartialUInt64SubOpV1 = PartialUIntSubOpV1<unsigned long long>;
        constexpr auto PartialInt64SubOpV1 = PartialIntSubOpV1<signed long long>;

        constexpr auto BasicSubtractByUInt = BasicSubtractByUIntV1<unsigned int>;
        constexpr auto BasicSubtractByInt = BasicSubtractByIntV1<signed int>;
        constexpr auto UnsignedBasicSubtractByInt = BasicSubtractByUIntV1<signed int>;
        constexpr auto BasicSubtractByUInt64 = BasicSubtractByUIntV1<unsigned long long>;
        constexpr auto BasicSubtractByInt64 = BasicSubtractByIntV1<signed long long>;
        constexpr auto UnsignedBasicSubtractByInt64 = BasicSubtractByUIntV1<signed int>;

        constexpr auto BasicSubtractByUInt8 = BasicSubtractByUIntV1<unsigned char>;
        constexpr auto BasicSubtractByInt8 = BasicSubtractByIntV1<signed char>;
        constexpr auto BasicSubtractByUInt16 = BasicSubtractByUIntV1<unsigned short>;
        constexpr auto BasicSubtractByInt16 = BasicSubtractByIntV1<signed short>;
	
//Replace with actual functions inside MediumDecV2 and other derived variants	
protected:
        /// <summary>
        /// Subtraction operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntSubOpV1 = BasicIntSubOpV1<IntType>;

public:

        constexpr auto UIntSubOpV1 = BasicUIntSubOpV1<unsigned int>;
        constexpr auto IntSubOpV1 = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSubOp = UnsignedBasicIntMultOp<signed int>;
        constexpr auto UInt64SubOp = BasicUIntSubOpV1<unsigned long long>;
        constexpr auto Int64SubOp = BasicIntSubOpV1<signed long long>;
	
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

protected:

        constexpr auto PartialSubOp = MediumDec::PartialSubOp;
        constexpr auto UnsignedPartialSubOp = MediumDec::UnsignedPartialSubOp;

public:

	
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedSubOp = MediumDec::BasicUnsignedSubOp;

		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicSubOp = MediumDec::BasicSubOp;
    
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = MediumDec::BasicUnsignedDivision;

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = MediumDec::BasicDivision;

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
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDec&</returns>
        constexpr auto SubtractByUnsigned = MediumDec::BasicUnsignedDivision;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Modifies owner object)
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
        friend MediumDec operator-(const MediumDec& self, const signed long long& Value) { return self.SubtractByInt64(Value); }
        friend MediumDec operator-(const MediumDec& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend MediumDec operator-(const MediumDec& self, const unsigned long long& Value) { return self.SubtractByUInt64(Value); }
		
        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }

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
        friend MediumDec& operator-=(MediumDec& self, const signed long long& Value) { return self.Int64SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned long long& Value) { return self.UInt64SubOp(Value); }

        friend MediumDec& operator-=(MediumDec& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend MediumDec& operator-=(MediumDec& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations

        /// <summary>
        /// Modulus Operation(Returning inside
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend ModResult operator%(const MediumDec& LValue, const MediumDec& RValue)
		{
			return ModResult(LValue, RValue);
		}

        friend MediumDec& operator%=(MediumDec& LValue, const MediumDec& RValue)
		{ 
            MediumDec divRes = LValue / RValue;
            LValue -= RValue * divRes;
			return *this;
		}

	#pragma endregion Modulus Operations
	
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

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The left side value</param>
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
        /// Bitwise XOR Operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
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
        /// Bitwise Or Operation between MediumDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
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