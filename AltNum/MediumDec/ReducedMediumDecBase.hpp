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
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like Pi(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API MediumDec : protected PartialMediumDec
    {
public:
		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModResult<PartialMediumDec>{};

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

        void SetVal(MediumDec Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

        template<MediumDecVariant VariantType=MediumDec>
        void SetVariantValue(MediumDec Value)
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
            IntHalf.Sign ^= 1;
        }

        void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = 0;
            DecimalHalf = 0;
        }

    #pragma region Const Representation values
    #pragma endregion Const Representation values
    
    #pragma region RepType
    #pragma endregion RepType

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

    #pragma region Comparison Operators
	
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

		bool operator==(const MediumDec& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.IntHalf)
				return false;
		}

		bool operator!=(const MediumDec& that) const
		{
			if (IntHalf!=that.IntHalf)
				return true;
			if (DecimalHalf!=that.IntHalf)
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

	#pragma region Other addition operations
	#pragma endregion Other addition operations

	#pragma region Other subtraction operations
	#pragma endregion Other subtraction operations

	#pragma region Modulus Operations
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
	#pragma endregion Bitwise Operations

    #pragma region Floating Operator Overrides
    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators

/*
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
*/

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Math Etc Functions
    #pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions
	#pragma endregion Pow and Sqrt Functions

    #pragma region Trigonomic Functions
    #pragma endregion Trigonomic Functions
    };

}
