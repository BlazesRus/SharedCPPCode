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
protected:
        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static signed int const NegativeRep = -2147483648;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        signed int IntValue;

        bool IsNegative()
        {
#if !defined(MediumDecBase_UseMirroredInt)
            return IntValue<0;
#else
            return IntValue.IsNegative();
#endif
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;
		
        /// <summary>
		/// (Used exclusively for alternative represents of numbers including imaginery numbers and for fractionals)
        /// If both DecimalHalf&ExtraRep are Positive with ExtraRep as non-zero, then ExtraRep acts as denominator
        /// If DecimalHalf is negative and ExtraRep is Positive, then MediumDecBase represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf is positive, then MediumDecBase represents +- 2147483647.999999999
        ///-----------------------------------------------
        /// If ExtraRep is negative, it acts as representation type similar to MediumDecBase:
		/// If ExtraRep is between 0 and , it acts as representation type similar to MediumDecBase:
        /// If DecimalHalf is positive and ExtraRep is -2147483648 and AltNum_EnablePiRep is enabled, then MediumDecBase represents +- 2147483647.999999999 * Pi
        /// </summary>
        signed int ExtraRep;

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
            return IntValue==0||IntValue==NegativeRep;
        }

        bool IsNotAtZeroInt()
        {
            return IntValue != 0 && IntValue != NegativeRep;
        }

        signed int GetIntHalf()
        {
#if defined(MediumDecBase_UseMirroredInt)
            return IntValue.GetValue();
#else
            if(IntValue == NegativeRep)
                return 0;
            else
                return IntValue;
#endif
        }

        //Return IntValue part as Absolute value
        signed int IntHalfAsAbs()
        {
#if defined(MediumDecBase_UseMirroredInt)
            return IntValue.GetAbsValue();
#else
            if (IsAtZeroInt())
                return 0;
            else if (IntValue < 0)
                return -IntValue;
            else
                return IntValue;
#endif
        }

        std::string IntHalfAsString()
        {
#if defined(MediumDecBase_UseMirroredInt)
            return (std::string) IntValue;
#else
            if (IntValue == NegativeRep)
                return "-0";
            return VariableConversionFunctions::IntToStringConversion(IntValue);
#endif
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        MediumDecBase(const IntHalfType& intVal, const signed int& decVal = 0, const DecimalHalfType& extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        MediumDecBase(const MediumDecBase&) = default;

        MediumDecBase& operator=(const MediumDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = rhs.ExtraRep;
            return *this;
        } const

        //Detect if at exactly zero
		bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		} const

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const MediumDecBase& Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        } const

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0; ExtraRep = 0;
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
	#if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
		//(other values only used if AltNum_EnableInfinityPowers is enabled)
		//If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
        static const signed int InfinityRep = -2147483648;
	#endif
	#if defined(AltNum_EnableApproachingValues)
        //Is Approaching Bottom when DecimalHalf==-2147483647:
        //If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)
		//If ExtraRep above 1 and 2147483645 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue from right towards left (IntValue.0__1)Pi
        static const signed int ApproachingBottomRep = -2147483647;
		//Is Approaching Top i when DecimalHalf==-2147483646:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
		//If ExtraRep above 1 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue+1 from left towards right (IntValue.9__9)Pi
		static const signed int ApproachingTopRep = -2147483646;
        #if defined(AltNum_EnableApproachingI)
		//Is Approaching Bottom i when DecimalHalf==-2147483645:
		//If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)i
        static const signed int ApproachingImaginaryBottomRep = -2147483645;
		//Is Approaching Top i when DecimalHalf==-2147483644:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)i
		static const signed int ApproachingImaginaryTopRep = -2147483644;
        #endif
	#endif
	#if defined(AltNum_EnablePiRep)
        //Is Pi*Value representation when ExtraRep==-2147483648
        static const signed int PiRep = -2147483648;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483645, then represents (IntValue/DecimalHalf)*Pi
        static const signed int PiByDivisorRep = -2147483645;
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*i 
        static const signed int IRep = -2147483647;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483644, then represents (IntValue/DecimalHalf)*i
        static const signed int IByDivisorRep = -2147483644;
		#endif
	#endif
	#if defined(AltNum_EnableERep)
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*e
        static const signed int ERep = -2147483646;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483643, then represents (IntValue/DecimalHalf)*e
        static const signed int EByDivisorRep = -2147483643;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)
		//Such as result of Cos of infinity
		//https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const signed int UndefinedInRangeRep = -2147483642;
		
		#if defined(AltNum_EnableWithinMinMaxRange)
		//Undefined but in ranged of IntValue to DecimalHalf
        static const signed int WithinMinMaxRangeRep = -2147483642;
		#endif
	#endif
        static const signed int AlternativeFractionalLowerBound = -2147483640;
		//Upper limit for Mixed Fractions; infinite approaching type representations at and after this DecimalHalf value
		static const signed int InfinityBasedLowerBound = -2147483644;
	#if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
	#endif

    #if defined(AltNum_EnableNil)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif

    #pragma endregion Const Representation values

    #pragma region RepType

    #pragma endregion RepType

    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMaximum()
        {
            IntValue = 2147483647; DecimalHalf = 999999999; ExtraRep = 0;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
            IntValue = -2147483647; DecimalHalf = 999999999; ExtraRep = 0;
        }
		
    #pragma endregion RangeLimits

	#pragma region PiNum Setters
    #if defined(AltNum_EnablePiRep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetPiValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        }
        #endif

        void SetPiVal(const MediumDecBase& Value)
        {
            if(ExtraRep==0)
            {
                IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                ExtraRep = PiRep;
            }
        }
        
        void SetPiVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = PiRep;
        }
    #endif
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
    #if defined(AltNum_EnableERep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetEValFromMediumDec(const MediumDec& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        }
        #endif

        void SetEVal(const MediumDecBase& Value)
        {
            if(ExtraRep==0)
            {
                IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                ExtraRep = ERep;
            }
        }
        
        void SetEVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = ERep;
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region Fractional Setters
    #if defined(AltNum_EnableFractionals)
        //Set value for NumByDiv
        void SetFractionalVal(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = IntHalf; DecimalHalf = IntHalf;
            ExtraRep = Divisor;
        }
		
        //Set value for NumByDiv
        void SetFractionalVal(MediumDecBase Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }

        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetFractionalVal(MediumDec Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }
        #endif
        
        //Set value for NumByDiv
        void SetFractionalVal(int Value, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = Divisor;
        }
    #endif
		
    #if defined(AltNum_EnableDecimaledPiFractionals)
        //Set value for PiNumByDiv
        void SetDecimaledPiFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = IntHalf; DecimalHalf = DecHalf;
            ExtraRep = -Divisor;
        }
	#elif defined(AltNum_EnableDecimaledEFractionals)
        //Set value for ENumByDiv
        void SetDecimaledEFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = -Divisor;
        }
	#elif defined(AltNum_EnableDecimaledIFractionals)
        //Set value for INumByDiv
        void SetDecimaledIFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = -Divisor;
        }
	#endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
	    #if defined(AltNum_EnablePiFractional)
        void SetPiFractional(int Num, int Divisor)//IntValue/DecimalHalf*Pi Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = PiRep;
        }
	    #endif
	    #if defined(AltNum_EnableEFractional)
        void SetEFractional(int Num, int Divisor)//IntValue/DecimalHalf*e Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = ERep;
        }
	    #endif
	    #if defined(AltNum_EnableIFractional)
        void SetIFractional(int Num, int Divisor)//IntValue/DecimalHalf*i Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = IRep;
        }
	    #endif
    #endif
    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
    #if defined(AltNum_EnableMixedFractional)
        void SetMixedFractionalVal(int WholeNum, int Numerator, int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = Numerator*-1;
            ExtraRep = Denom;
        }
        
        void SetMixedFractionalValAsNeg(int WholeNum, int NumeratorAsNeg, int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = NumeratorAsNeg;
            ExtraRep = Denom;
        }
    #endif
    #pragma endregion MixedFrac Setters
	
    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        void SetAsInfinity()
        {
            IntValue = 1; DecimalHalf = InfinityRep;
            ExtraRep = 0;
        }

        void SetAsNegativeInfinity()
        {
            IntValue = -1; DecimalHalf = InfinityRep;
            ExtraRep = 0;
        }
	#endif
    #pragma endregion Infinity Setters
	
    #pragma region ApproachingZero Setters
	#if defined(AltNum_EnableApproachingValues)
		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
		//If AltNum_EnableApproachingDivided is enabled and Divisor value is greator than 1, Approaching Fractional from left;ExtraRep value of 2 results in value.499...9(for positive value:value.(1/Divisor-JustAboveZero))
		#if defined(AltNum_EnableApproachingDivided)
		void SetAsApproachingBottom(int value=0, int Divisor=0)
		#else
        void SetAsApproachingBottom(int value=0)
		#endif
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
		#if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
		#else
            ExtraRep = 0;
		#endif
        }
        
		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
		//If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
			#if defined(AltNum_EnableApproachingDivided)
		void SetAsApproachingTop(int value, int Divisor=0)
			#else
        void SetAsApproachingTop(int value)
			#endif
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
			#if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
			#else
            ExtraRep = 0;
			#endif
        }
		
			#if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
		//If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
        void SetAsApproachingTopPi(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = PiRep;
        }
			#endif
			
			#if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
		//If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
        void SetAsApproachingTopPi(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = ERep;
        }
			#endif
		#endif
	#endif

	#if defined(AltNum_EnableApproachingI)
        //Approaching Towards values from right to left side(IntValue.000...1)i
		//If AltNum_EnableApproachingDivided is enabled and Divisor value is greator than 1, Approaching Fractional from left;ExtraRep value of 2 results in value.499...9(for positive value:value.(1/Divisor-JustAboveZero))
			#if defined(AltNum_EnableApproachingDivided)
		void SetAsApproachingImaginaryBottom(int value=0, int Divisor=0)
			#else
        void SetAsApproachingImaginaryBottom(int value=0)
			#endif
        {
            IntValue = value; DecimalHalf = ApproachingImaginaryBottomRep;
			#if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
			#else
            ExtraRep = 0;
			#endif
        }
        
		#if !defined(AltNum_DisableApproachingTop)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
		//If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
			#if defined(AltNum_EnableApproachingDivided)
		void SetAsApproachingImaginaryTop(int value, int Divisor=0)
			#else
        void SetAsApproachingImaginaryTop(int value)
			#endif
        {
            IntValue = value; DecimalHalf = ApproachingImaginaryTopRep;
			#if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
			#else
            ExtraRep = 0;
			#endif
        }
		#endif
	#endif
    #pragma endregion ApproachingZero Setters
		
	#pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
            ExtraRep = 0;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
            ExtraRep = 0;
        }
	#endif
    #pragma endregion NaN Setters

    #pragma region ValueDefines
    protected:
	#if defined(AltNum_EnableNaN)
        static MediumDecBase NaNValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, NaNRep);
            return NewSelf;
        }
		
        static MediumDecBase UndefinedValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, UndefinedRep);
            return NewSelf;
        }
	#endif

	#if defined(AltNum_EnableInfinityRep)
        static MediumDecBase InfinityValue()
        {
            MediumDecBase NewSelf = MediumDecBase(1, InfinityRep);
            return NewSelf;
        }
        
        static MediumDecBase NegativeInfinityValue()
        {
            MediumDecBase NewSelf = MediumDecBase(-1, InfinityRep);
            return NewSelf;
        }
	#endif
        
	#if defined(AltNum_EnableApproachingValues)
        static MediumDecBase ApproachingZeroValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, ApproachingBottomRep);
            return NewSelf;
        }
		
        static MediumDecBase ApproachingOneFromLeftValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, ApproachingTopRep);
            return NewSelf;
        }
		
        static MediumDecBase ApproachingOneFromRightValue()
        {
            MediumDecBase NewSelf = MediumDecBase(1, ApproachingBottomRep);
            return NewSelf;
        }

        static MediumDecBase NegativeApproachingZeroValue()
        {
            MediumDecBase NewSelf = MediumDecBase(NegativeRep, ApproachingBottomRep);
            return NewSelf;
        }
	#endif
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase PiNumValue()
        {
            return MediumDecBase(3, 141592654);
        }
        
        static MediumDecBase PiValue()
        {
#if defined(AltNum_EnableERep)
            return MediumDecBase(1, PiRep);
#else
            return MediumDecBase(3, 141592654);
#endif
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase HundredMilPiNumVal()
        {
            return MediumDecBase(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase TenMilPiNumVal()
        {
            return MediumDecBase(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase OneMilPiNumVal()
        {
            return MediumDecBase(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecBase TenPiNumVal()
        {
            return MediumDecBase(31, 415926536);
        }
        
        static MediumDecBase ENumValue()
        {
            return MediumDecBase(2, 718281828);
        }

        static MediumDecBase EValue()
        {
#if defined(AltNum_EnableERep)
            return MediumDecBase(1, ERep);
#else
            return MediumDecBase(2, 718281828);
#endif
        }
        
        static MediumDecBase ZeroValue()
        {
            return MediumDecBase();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase OneValue()
        {
            MediumDecBase NewSelf = MediumDecBase(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase TwoValue()
        {
            MediumDecBase NewSelf = MediumDecBase(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase NegativeOneValue()
        {
            MediumDecBase NewSelf = MediumDecBase(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Point5Value()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 500000000);
            return NewSelf;
        }

        static MediumDecBase JustAboveZeroValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 1);
            return NewSelf;
        }

        static MediumDecBase OneMillionthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 1000);
            return NewSelf;
        }

        static MediumDecBase FiveThousandthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 5000000);
            return NewSelf;
        }

        static MediumDecBase FiveMillionthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 5000);
            return NewSelf;
        }

        static MediumDecBase TenMillionthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 100);
            return NewSelf;
        }

        static MediumDecBase OneHundredMillionthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 10);
            return NewSelf;
        }

        static MediumDecBase FiveBillionthValue()
        {
            MediumDecBase NewSelf = MediumDecBase(0, 5);
            return NewSelf;
        }

        static MediumDecBase LN10Value()
        {
            return MediumDecBase(2, 302585093);
        }

        static MediumDecBase LN10MultValue()
        {
            return MediumDecBase(0, 434294482);
        }

        static MediumDecBase HalfLN10MultValue()
        {
            return MediumDecBase(0, 868588964);
        }
        
    #if defined(AltNum_EnableNil)
        static MediumDecBase NilValue()
        {
            return MediumDecBase(NilRep, NilRep);
        }
    #endif

        static MediumDecBase MinimumValue()
        {
            return MediumDecBase(2147483647, 999999999);
        }

        static MediumDecBase MaximumValue()
        {
            return MediumDecBase(2147483647, 999999999);
        }
public:
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
        
#if defined(AltNum_EnableInfinityRep)
        static MediumDecBase Infinity;
        static MediumDecBase NegativeInfinity;
        static MediumDecBase ApproachingZero;
#endif

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
        static MediumDecBase LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecBase HalfLN10Mult;

    #if defined(AltNum_EnableNil)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDecBase Nil;
    #endif
	
#if defined(AltNum_EnableApproachingValues)
        static MediumDecBase ApproachingRightRealValue(int IntValue=0)
        {
            return MediumDecBase(IntValue, 999999999);
        }

        static MediumDecBase ApproachingLeftRealValue(int IntValue=0)
        {
            return MediumDecBase(IntValue, 1);
        }

        static MediumDecBase LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return MediumDecBase(IntValue, 499999999);
        }

        static MediumDecBase RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return MediumDecBase(IntValue, 500000001);
        }
#endif
	
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

private:
        std::string BasicToStringOp();
		
		std::string BasicToFullStringOp();
public:

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToString();

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToFullString();

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }
    #pragma endregion String Commands

    #pragma region From Standard types to this type
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(float Value)
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
        void SetVal(double Value)
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
        void SetVal(ldouble Value)
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
        void SetVal(bool Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(float Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(double Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(ldouble Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(bool Value)
        {
            this->SetVal(Value);
        }

    #if defined(AltNum_EnableMediumDecBasedSetValues)
        MediumDecBase(MediumDec Value)
        {
            this->SetVal(Value);
        }
    #endif
    #pragma endregion From Standard types to this type

    #pragma region From this type to Standard types
        /// <summary>
        /// MediumDecBase to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            if (IntValue < 0)
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = (float)IntValue;
    #else
                Value = (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecBase to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            if (IntValue < 0)
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = (double)IntValue;
    #else
                Value = (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecBase to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            if (IntValue < 0)
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
    #if !defined(MediumDecBase_UseMirroredInt)
                Value = (ldouble)IntValue;
    #else
                Value = (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }
		
        /// <summary>
        /// MediumDecBase to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return GetIntHalf(); }

        explicit operator bool() { return IntValue == 0 ? false : true; }
    #pragma endregion From this type to Standard types

    #pragma region Pi Conversion
	#if defined(AltNum_EnablePiRep)
		//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
        void ConvertPiToNum();
	
		#if defined(AltNum_EnableDecimaledPiFractionals)
        void ConvertPiByDivToNumByDiv();
		
        void ConvertFromPiByDivToNorm();
		#else
		void ConvertFromPiFractionalToNorm();
		
		#endif
		
        void ConvertPiPowerToNum();
		
        MediumDecBase PiPowerNum(int powerExponent);
		
        void ConvertPiPowerToPiRep();
		
        void ConvertToPiRep(const RepType& repType)
        {
            switch (repType)
            {
                case RepType::PiNum:
                    return;
                    break;
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ConvertPiPowerToPiRep();
                    break;
    #endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                {
                    BasicUIntDivOp(-ExtraRep);
                }
    #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicUnsignedDivOp(divisor);
                }
    #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = PiRep;
        }

        MediumDecBase ConvertAsPiRep(const RepType& repType)
        {
            switch (repType)
            {
                case RepType::PiNum:
                    return *this;
                    break;
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                {
                    MediumDecBase Res = *this;
                    Res.ConvertPiPowerToPiRep();
                    return Res;
                    break;
                }
    #endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                {
                    MediumDecBase Res = MediumDecBase(IntValue, DecimalHalf, PiRep);
                    Res.BasicUIntDivOp(-ExtraRep);
                    return Res;
                }
    #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {
                    MediumDecBase Res = MediumDecBase(IntValue, 0, PiRep);
                    Res.BasicUIntDivOp(DecimalHalf);
                    return Res;
                }
    #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = PiRep;
        } const
	#endif
    #pragma endregion Pi Conversion
	
    #pragma region E Conversion
	#if defined(AltNum_EnableERep)
		//2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
        void ConvertENumToNum();
		
		#if defined(AltNum_EnableDecimaledEFractionals)
		void ConvertEByDivToNumByDiv();

        void ConvertFromEByDivToNorm();
		
		//void ConvertFromEByDivToNorm();
		#else
        void ConvertFromEFractionalToNorm();
		#endif
        void ConvertToERep(const RepType& repType)
        {
            switch (repType)
            {
                case RepType::ENum:
                    return;
                    break;
    #if defined(AltNum_EnableAlternativeRepFractionals)
    #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
                {
                    BasicUIntDivOp(-ExtraRep);
                }
    #else
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
    #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = ERep;
        }

        MediumDecBase ConvertAsERep(const RepType& repType)
        {
            switch (repType)
            {
                case RepType::ENum:
                    return *this;
                    break;
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
                {
                    MediumDecBase Res = MediumDecBase(IntValue, DecimalHalf, ERep);
                    Res.BasicUIntDivOp(-ExtraRep);
                    return Res;
                }
        #else
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    MediumDecBase Res = MediumDecBase(IntValue, 0, ERep);
                    Res.BasicUIntDivOp(DecimalHalf);
                    return Res;

                }
        #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = ERep;
        } const
	#endif
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
#if defined(AltNum_EnableApproachingDivided)
		void ConvertFromApproachingMidLeftToNorm()
		{//0.9..9/2 = 0.49..9
			DecimalHalf = (DecimalOverflow / ExtraRep) - 1;
			ExtraRep = 0;
		}
		
	#if !defined(AltNum_DisableApproachingTop)
		void ConvertFromApproachingMidRightToNorm()
		{
			DecimalHalf = (DecimalOverflow / ExtraRep) + 1;
			ExtraRep = 0;
		}
	#endif

    #if defined(AltNum_EnableApproachingI)
		void ConvertFromApproachingIMidLeftToINum()
		{
			DecimalHalf = (DecimalOverflow / ExtraRep) - 1;
			ExtraRep = IRep;
		}
	
	#if !defined(AltNum_DisableApproachingTop)
		void ConvertFromApproachingIMidRightToINum()
		{
			DecimalHalf = (DecimalOverflow / ExtraRep) + 1;
			ExtraRep = IRep;
		}
	#endif
    #endif
#endif
	
        //Switch based version of ConvertToNormType(use ConvertAsNormType instead to return converted value without modifying base value)
        void ConvertToNormType(const RepType& repType); const

        //Switch based return of value as normal type representation
        MediumDecBase ConvertAsNormType(const RepType& repType); const

		//Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
		
		//Returns value as normal type representation
        virtual MediumDecBase ConvertAsNormTypeV2()
        {
            MediumDecBase Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }
    #if defined(AltNum_EnableImaginaryNum)
        void ConvertIRepToNormal(const RepType& repType)
        {//Assuming not zero(should not reach needing to convert the representation if RValue is zero)
            switch (repType)
            {
                case RepType::INum:
                {
                    break;
                }
                    #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
                {
                    int Divisor = -ExtraRep;
                    BasicUnsignedDivOp(Divisor);
                    break;
                }
                #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    int Divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicIntDivOp(Divisor);
                    break;
                }
                #endif
                #ifdef AltNum_EnableComplexNumbers
                case RepType::ComplexIRep:
                {
                    throw "Conversion from complex number to real number not supported yet.";
                    break;
                }
    #endif
                default:
                    throw "Conversion not supported.";
                    break;
            }
        } const
		
		void ConvertToNormalIRep(const RepType& repType)
        {//Assuming not zero(should not reach needing to convert the representation)
            ConvertIRepToNormal(repType);
            ExtraRep = IRep;
        } const
		
		MediumDecBase ConvertAsNormalIRep(const RepType& repType)
        {
            MediumDecBase Res = *this;
            Res.ConvertToNormalIRep(repType);
            return Res;
        } const
		
		void ConvertToNormalIRepV2()
        {
	        RepType repType = GetRepType();
	        ConvertToNormalIRep(repType);
        }

		void ConvertIRepToNormalV2()
        {
	        RepType repType = GetRepType();
	        ConvertIRepToNormal(repType);
        }

		MediumDecBase ConvertIRepAsNormalV2()
        {
	        MediumDecBase Res = *this;
	        RepType repType = GetRepType();
	        Res.ConvertIRepToNormal(repType);
	        return Res;
        }
		
        MediumDecBase ConvertAsNormalIRepV2()
        {
	        MediumDecBase Res = *this;
	        Res.ConvertToNormalIRepV2();
	        return Res;
        }

    #endif
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators

protected:
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = MediumDecBase::BasicComparisonV1<MediumDecV2Base>;

#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = MediumDecBase::BasicComparisonWithoutSignCheck<MediumDecV2Base>;
#endif

    #if defined(AltNum_DefineInfinityAsSignedReps)
        constexpr auto LSideInfinityComparison = MediumDecV2Base::LSideInfinityComparison<AltDecBase>;
	#endif

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=AltDecBase>
		std::strong_ordering CompareWithV1(const VariantType& that) const
		{
	#if defined(AltNum_EnableWithinMinMaxRange)
			if(ExtraRep==WithinMinMaxRangeRep) {
				if(ExtraRep==WithinMinMaxRangeRep) {
					//To-do compare within min-max range code here
				}
				else {
					//To-do compare within min-max range code here
				}
			}
			else if(ExtraRep==WithinMinMaxRangeRep) {
				//To-do compare within min-max range code here
			}
	#endif
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
	#endif
	
			RepType LRep = GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(AltNum_EnableNaN)||defined(AltNum_EnableNilRep)||defined(AltNum_EnableUndefinedButInRange)
			if(LRep^UndefinedBit||RRep^UndefinedBit)
				throw "Can't compare undefined/nil representations";
    #endif
	#if defined(AltNum_UseIntForDecimalHalf)
		//To-Do add code here
	#else
		#if defined(AltNum_EnableImaginaryNum)
            if (DecimalHalf.Flags == 3)
            {
                if(that.DecimalHalf.Flags!=3)
                    throw "Can't compare imaginary number with real number";
				else if(RRep==RepType:ImaginaryInfinity)
                {
					if(that.IntValue==1)
						return 0<=>1;//Positive Infinity is greater than real number representations
					else
						return 1<=>0;
                }
                else
                {
					VariantType LValue = this;
					VariantType RValue = that;
			#if defined(AltNum_EnablePowerOfRepresentation)
				#if defined(AltNum_EnableNegativePowerRep)
					int LComp = (int)LValue.ExtraRep;
					int RComp = (int)RValue.ExtraRep;
					if(LComp!=0)//Left side is to power of ExtraRep.Value
					{
						if(RComp!=0)//Right side is to power of ExtraRep.Value
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(RComp!=0)//Right side is to power of ExtraRep.Value
					{
						//Add code here
					}
				#else
					if(ExtraRep.IsPositive==0)//Left side is to power of ExtraRep.Value
					{
						if(that.ExtraRep.IsPositive)//Right side is to power of ExtraRep.Value
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.IsPositive)//Right side is to power of ExtraRep.Value
					{
						//Add code here
					}
				#endif
			#elif defined(AltNum_EnableMixedFractional)
					if(ExtraRep.IsPositive==0)//Left side is a mixed Fraction
					{
						if(that.ExtraRep.IsPositive)//Right side is a mixed Fraction
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.IsPositive)//Right side is a mixed Fraction
					{
						//Add code here
					}
			#endif
					else if(ExtraRep.Value!=0)//Left side is a divisor
					{
						if(that.ExtraRep.Value!=0)//Right side is a divisor
						{
							//Add code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.Value!=0)//Right side is a divisor
					{
						//Add code here
					}
			#if defined(AltNum_EnableMirroredSection)
			    	return BasicComparisonV2(rSide);
			#else
					return BasicComparison(rSide);
			#endif
                }
            }
			else if(that.Flags==3)
				throw "Can't compare imaginary number with real number";
		#endif
	#endif
			switch(LRep)
			{
	#if defined(AltNum_EnableInfinityRep)
                case RepType:Infinity:
                    LSideInfinityComparison(that, RRep);
                    break;
	#endif
	#if defined(AltNum_EnableApproachingValues)
	
	#endif
	#if defined(AltNum_EnableFractionals)
				case RepType:NumByDiv:
		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType:PiNumByDiv:
		#endif
		#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType:ENumByDiv:
		#endif
		#if defined(AltNum_EnablePiFractional)
				case RepType:PiFractional:
		#endif
		#if defined(AltNum_EnableEFractional)
				case RepType:EFractional:
		#endif
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(AltNum_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType:ToPowerOf:
				case RepType:PiPower:
				case RepType:EPower:
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(AltNum_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
				case RepType:MixedFrac:
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType:MixedPi:
		#endif
		#if defined(AltNum_EnableMixedEFractional)
				case RepType:MixedE:
		#endif
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(AltNum_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
				default:
				{
					if(LRep==RRep)
	#if defined(AltNum_EnableMirroredSection)
						return BasicComparisonV2(that);
	#else
						return BasicComparison(that);
	#endif
					else if(RRep==RepType:Infinity)
                    {
                        if(that.IntValue==1)
							return 0<=>1;//Positive Infinity is greater than real number representations
						else
							return 1<=>0;
                    }
                    else
					{
						MediumDecV2Base lSide = *this;
						MediumDecV2Base rSide = that;
						lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
	#if defined(AltNum_EnableMirroredSection)
						return lSide.BasicComparisonV2(rSide);
	#else
						return rSide.BasicComparison(rSide);
	#endif
					}
				}
			}
		}

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering CompareWithIntV1(const int& that) const
		{
			int lVal; int rVal;
			//Pi and E only enabled if imbedded flags are enabled
	#if !defined(AltNum_UseIntForDecimalHalf)
			if(DecimalHalf.Flags==0)
	#else
			if(ExtraRep==0)
	#endif
			{
				return BasicIntComparison(that);
			}
	#if defined(AltNum_EnableImaginaryNum
		#if !defined(AltNum_UseIntForDecimalHalf)
			else if(DecimalHalf.Flags==3)
		#elif defined(AltNum_EnableDecimaledIFractionals)//Check if within I Fractional Representation or INum representation to check if valid
			else if((ExtraRep<0&&ExtraRep>FractionalDivisorOverflow)||ExtraRep==IRep)
		#else
			else if(ExtraRep==IRep)
		#endif
				throw "Can't compare imaginary number with real number";
	#endif
			else
			{
				MediumDecV2Base lSide = *this;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicIntComparison(that);
			}
	#endif
		}

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWith = MediumDecBase::CompareWithV1<MediumDecV2Base>;

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWithInt = MediumDecBase::CompareWithIntV1<MediumDecV2Base>;

public:
		std::strong_ordering operator<=>(const MediumDecV2Base& that) const
		{
			return CompareWith(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return CompareWithInt(that);
		}

		bool operator==(const int& that) const
		{
			if (IntValue!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator==(const MediumDecV2Base& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:

        template<IntegerType IntType=signed int>
        bool PartialIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
			if(rValue<0)
			{
				ResIsNegative = !ResIsNegative;
				IntType invertedValue = -rValue;
                Res = SelfRes / invertedValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - invertedValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
			}
			else
			{
                Res = SelfRes / rValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - rValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
			}
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
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

        template<IntegerType IntType=signed int>
        bool PartialUIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            signed _int64 TruncatedDigits; 
        #endif
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
            //One has SelfRes of 1000000000
            //1000000000/2 = 500000000
            Res = SelfRes / rValue;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            TruncatedDigits = SelfRes - rValue * IntHalfRes;
        #endif
            IntHalfRes = SelfRes/DecimalOverflowX;
            DecimalRes = SelfRes - DecimalOverflowX * IntHalfRes;
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
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

public:

        template<IntegerType IntType=signed int>
        MediumDecBase& BasicUIntDivOp(const IntType& rValue)
        {
            if (rValue == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
            return *this;
        }

        template<IntegerType IntType=signed int>
        void BasicIntDivOp(const IntType& rValue)
        {
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            if (rValue < 0)
            {
                IntType invertedValue = -rValue;
                SwapNegativeStatus();
                if(PartialUIntDivOp(invertedValue))
                    DecimalHalf = 1;//Prevent Dividing into nothing
            }
            else if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
        }

        void DivideByTwo()
        {
            if(IntValue^1==1)//Check if number is odd
                rValue.BasicIntDivOp(2);
            else
                IntValue /= 2;
        }

		void BasicInt32DivOp(signed int& rValue) { BasicIntDivOp(rValue); }
		void BasicInt64DivOp(signed long long& rValue) { BasicIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicDivideByUIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicUIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicDivideByIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicIntDivOp(Value); }

        /// <summary>
        /// Division Operation Between MediumDecBase and Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicDivideByUInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicUIntDivOp(rValue); }
        
        /// <summary>
        /// Division Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicDivideByInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicIntDivOp(Value); }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations

protected:
		/// <summary>
        /// Partial Multiplication Operation Between MediumDecBase and Integer Value
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType=signed int>
        void PartialIntMultOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
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
                if(rValue<0)
                {
                    SelfIsNegative = !SelfIsNegative;
                    SRep *= -rValue;
                }
                else
                    SRep *= rValue;
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
        /// Partial Multiplication Operation Between MediumDecBase and Integer Value
        /// Applies after making sure rValue is positive
        /// (Modifies owner object)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType=signed int>
        void PartialIntMultOpV2(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
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
                SRep *= rValue;
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

public:
        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
        MediumDecBase BasicIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOp(rValue);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
        void BasicUIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOpV2(rValue);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType = int>
        MediumDecBase BasicIntMult(const IntType& rValue) { MediumDecBase self = *this; self.BasicIntMultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType = int>
        void BasicUIntMult(const IntType& rValue) { MediumDecBase self = *this; self.BasicUIntMultOp(rValue); return self; }

		void BasicInt32MultOp(signed int& rValue) { BasicIntMultOp(rValue); }
		void BasicInt64MultOp(signed long long& rValue) { BasicIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicMultipleByUIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicUIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicMultipleByIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value that ignores special representation status
        /// (lValue is copied variable) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicMultipleByUInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicUIntMultOp(rValue); }
        
        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (lValue is copied variable) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicMultipleByInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicIntMultOp(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

protected:
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

public:

        /// <summary>
        /// Basic Addition Operation between MediumDecBase and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& BasicIntAddOp(const IntType& rValue)
        {
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(rValue);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfAdditionOp(rValue);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
            }
            return *this;
        }

        /// <summary>
        /// Addition Operation Between MediumDecBase and  Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicAddByIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicIntAddOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicAddByInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicIntAddOp(rValue); }
	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

protected:
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

public:

		/// <summary>
        /// Basic Subtraction Operation between MediumDecBase and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase BasicIntSubOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                NRepSkippingIntSubOp(Value);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfSubtractionOp(rValue);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
            }
            return *this;
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicSubtractByIntOp(MediumDecBase& lValue, const IntType& rValue) { return lValue.BasicIntSubOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=signed int>
		static MediumDecBase BasicSubtractByInt(MediumDecBase lValue, const IntType& rValue) { return lValue.BasicIntSubOp(rValue); }

    #pragma endregion NormalRep Integer Subtraction Operations
	
    #pragma region NormalRep Integer Bitwise Operations

	#pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations
    #if defined(AltNum_EnableMixedFractional)
		//Assumes NormalRep + Normal MixedFraction operation
		void BasicMixedFracAddOp(MediumDecBase& rValue)
		{
			if(DecimalHalf==0)//Avoid needing to convert if Leftside is not decimal format representation
			{
				if(IntValue<0)
				{
					if(rValue.IntValue==NegativeRep)
					{
						DecimalHalf = rValue.DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
					else if(rValue.IntValue<0)
					{
						IntValue += rValue.IntValue;
						DecimalHalf = rValue.DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
					else//(rValue.IntValue>0)
					{
						if(rValue.IntValue>-IntValue)//check for flipping of sign
						{
							IntValue += rValue.IntValue - 1;
							DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
						}
						else
						{
							IntValue += rValue.IntValue;
							DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
						}
						ExtraRep = rValue.ExtraRep;
					}
				}
				else//(IntValue>0)
				{
					if(rValue.IntValue==NegativeRep)
					{
						DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
					else if(rValue.IntValue<0)
					{
						IntValue += rValue.IntValue;
						if(-rValue.IntValue>IntValue)//check for flipping of sign
						{
							IntValue += rValue.IntValue;
							if(IntValue==-1)
								IntValue = NegativeRep;
							else
								++IntValue;
						}
						DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
					else//(rValue.IntValue>0)
					{
						IntValue += rValue.IntValue;
						DecimalHalf = rValue.DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
				}     
			}
			else
			{
				MediumDecBase RightSideNum = MediumDecBase(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
				BasicIntMultOp(rValue.ExtraRep);
                BasicAddOp(RightSideNum);//self += RightSideNum;
				if(DecimalHalf==0)
				{
					if(IntValue!=0)//Set as Zero if both are zero
					{
						DecimalHalf = -DecimalHalf;
						ExtraRep = rValue.ExtraRep;
					}
				}
				else
				{
					if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
						DecimalHalf = -DecimalHalf;
					ExtraRep = rValue.ExtraRep;
				}
			}
		}
		
	#if defined(AltNum_EnableMixedPiFractional)
		void BasicMixedPiFracAddOp(MediumDecBase& rValue)
	#elif defined(AltNum_EnableMixedEFractional)
		void BasicMixedEFracAddOp(MediumDecBase& rValue)
	#endif
	#if defined(AltNum_MixedPiOrEEnabled)
		{
			MediumDecBase RightSideNum = MediumDecBase(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
		#if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		#else
			RightSideNum *= ENum;
		#endif
            BasicIntMultOp(-rValue.ExtraRep);
            BasicAddOp(RightSideNum);
			if(DecimalHalf==0)
			{
				if(IntValue!=0)//Set as Zero if both are zero
				{
					DecimalHalf = -DecimalHalf;
					ExtraRep = -rValue.ExtraRep;
				}
			}
			else
			{
				if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					DecimalHalf = -DecimalHalf;
				ExtraRep = -rValue.ExtraRep;
			}
		}
	#endif
		
		//Assumes NormalRep - Normal MixedFraction operation
		void BasicMixedFracSubOp(MediumDecBase& rValue)
		{
			MediumDecBase RightSideNum = MediumDecBase(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
			BasicMultOp(rValue.ExtraRep);
			BasicSubOp(RightSideNum);
			if(DecimalHalf==0)
			{
				if(IntValue!=0)//Set as Zero if both are zero
				{
					DecimalHalf = -DecimalHalf;
					ExtraRep = rValue.ExtraRep;
				}
			}
			else
			{
				if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					DecimalHalf = -DecimalHalf;
				ExtraRep = rValue.ExtraRep;
			}
		}
		
	#if defined(AltNum_EnableMixedPiFractional)
		void BasicMixedPiFracSubOp(MediumDecBase& rValue)
	#elif defined(AltNum_EnableMixedEFractional)
		void BasicMixedEFracSubOp(MediumDecBase& rValue)
	#endif
	#if defined(AltNum_MixedPiOrEEnabled)
		{
			MediumDecBase RightSideNum = MediumDecBase(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
		#if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		#else
			RightSideNum *= ENum;
		#endif
			BasicMultOp(-rValue.ExtraRep);
			BasicSubOp(RightSideNum);
			if(DecimalHalf==0)
			{
				if(IntValue!=0)//Set as Zero if both are zero
				{
					DecimalHalf = -DecimalHalf;
					ExtraRep = -rValue.ExtraRep;
				}
			}
			else
			{
				if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					DecimalHalf = -DecimalHalf;
				ExtraRep = -rValue.ExtraRep;
			}
		}
	#endif
#endif
    #pragma endregion Mixed Fraction Operations

	#pragma region NormalRep AltNum Division Operations
protected:
		/// <summary>
        /// Basic Division Operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param
        bool PartialDivOp(const MediumDecBase& rValue)
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
                if (rValue < 0)
                {
                    if (rValue.IntValue == NegativeRep)
                        ValueRes = rValue.DecimalHalf;
                    else
                        ValueRes =  NegDecimalOverflowX* rValue.IntValue + rValue.DecimalHalf;
                }
				else
				{
				    ResIsPositive = false;
					ValueRes = DecimalOverflowX * rValue.IntValue +rValue.DecimalHalf;
				}
			}
			else
			{
                
				SelfRes = DecimalOverflowX* IntValue+DecimalHalf;
			    if(rValue<0)
				{
				    ResIsPositive = false;
					ValueRes = rValue.IntValue==NegativeRep ? DecimalHalf: NegDecimalOverflowX*IntValue +rValue.DecimalHalf;
				}
				else
					ValueRes = DecimalOverflowX* rValue.IntValue +rValue.DecimalHalf;
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

        bool UnsignedPartialDivOp(const MediumDecBase& rValue)
        {
            bool ResIsPositive = true;
            signed _int64 SelfRes;
            signed _int64 ValueRes;
            if (IntValue < 0)
            {
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
                    SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                ResIsPositive = false;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }
            else
            {

                SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }

            signed _int64 IntHalfRes = SelfRes / ValueRes;
            signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
            IntValue = IntHalfRes == 0 && ResIsPositive == false ? NegativeRep : IntHalfRes;
            DecimalHalf = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
public:

		/// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        MediumDecBase& BasicUnsignedDivOp(const MediumDecBase& rValue)
        {
			if (PartialDivOp(rValue))//Prevent Dividing into nothing
				DecimalHalf = 1;
            return *this;
        }

		/// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        MediumDecBase& BasicUnsignedDivOp(const MediumDecBase& rValue)
        {
            if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf = 1;
            return *this;
        }

        //Performs division operation ignoring non-normal representation with right side MediumDecBase
        MediumDecBase BasicDiv(const MediumDecBase& rValue)
        { MediumDecBase self = *this; self.BasicUnsignedDivOp(rValue); return self; }

        //Performs division operation (without checking negative) ignoring non-normal representation with right side MediumDecBase
        MediumDecBase BasicUnsignedDiv(const MediumDecBase& rValue) 
        { MediumDecBase self = *this; self.BasicUnsignedDivOp(rValue);  return self; }

        MediumDecBase BasicDivision(MediumDecBase self, const MediumDecBase& rValue)
        {
            if (self.PartialDivOp(rValue))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

        MediumDecBase BasicUnsignedDivision(MediumDecBase self, const MediumDecBase& rValue)
        {
            if (self.UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

	#pragma endregion NormalRep AltNum Division Operations
        
	#pragma region NormalRep AltNum Multiplication Operations
public:
		
		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
		MediumDecBase& BasicMultOp(const MediumDecBase& rValue)
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
                    __int64 rRep;
                    bool ResIsNegative = rValue < 0;
                    if(ResIsNegative)
                    {
                        rRep = rValue.IntValue == NegativeRep ? rValue.DecimalHalf : DecimalOverflowX * -rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = false;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = true;
                            rRep *= IntValue;
                        }
                    }
                    else
                    {
                        rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = true;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = false;
                            rRep *= IntValue;
                        }
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
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
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
                if (IntValue == 0)
                {
                    __int64 SRep = (__int64)DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= MediumDecBase::DecimalOverflowX;
                    if (rValue.IntValue == 0)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative==false&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        SRep += (__int64)DecimalHalf * rValue.IntValue;
                        if (SRep >= MediumDecBase::DecimalOverflowX)
                        {
                            __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                            SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                            IntValue = SelfIsNegative?-(int)OverflowVal:OverflowVal;
                            DecimalHalf = (signed int)SRep;
    						return *this;
                        }
                        else
                        {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                if(ExtraRep!=0)
                                    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            if(SelfIsNegative)
                                IntValue = NegativeRep;
                            return *this;
                        }
                    }
                }
                else if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    if (rValue < 0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                if(ExtraRep!=0)
                                    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                            return *this;
                    }
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
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                        return *this;
                    }
                }
                else if (rValue.IntValue == NegativeRep)
                {
                    SelfIsNegative = !SelfIsNegative;
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= MediumDecBase::DecimalOverflowX;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
                        return *this;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    //X.Y *.V
                    if(rValue<0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    
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
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
#else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned MediumDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
		MediumDecBase& BasicUnsignedMultOp(const MediumDecBase& rValue)
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
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntValue == 0)
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
                #else
                    if (DecimalHalf == 0&&ExtraRep!=0)
                        ExtraRep = 0;
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
                #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0&&ExtraRep!=0)
                            ExtraRep = 0;
                #else
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
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
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        return *this;
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
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = MediumDecBase::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= MediumDecBase::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecBase::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecBase::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
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
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? MediumDecBase::NegativeRep : 0;
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
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
#else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

        MediumDecBase BasicMult(const MediumDecBase& rValue) { MediumDecBase self = *this; self.BasicUnsignedDivOp(rValue); return self; }

        MediumDecBase BasicUnsignedMult(const MediumDecBase& rValue) { MediumDecBase self = *this; self.BasicUnsignedMultOp(rValue); return self; }

        MediumDecBase BasicMultiplication(MediumDecBase self, const MediumDecBase& rValue)
        {
            return self.BasicMultOp(rValue);
        }

        MediumDecBase BasicUnsignedMultiplication(MediumDecBase self, const MediumDecBase& rValue)
        {
            return self.BasicUnsignedMultOp(rValue);
        }

        template<typename MediumDecBaseVariant = MediumDecBase>
        MediumDecBase BasicMult(const MediumDecBase& rValue) { MediumDecBase self = *this; self.BasicMultOp(rValue); return self; }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const MediumDecBase& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAdditionOp(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += rValue.DecimalHalf;
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
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const MediumDecBase& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAdditionOp(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (WasNegative)
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                }
                else
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
        }

	#pragma endregion NormalRep AltNum Addition Operations
        
	#pragma region NormalRep AltNum Subtraction Operations

        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
		void BasicSubOp(const MediumDecBase& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtractionOp(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= rValue.DecimalHalf;
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
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
		void BasicUnsignedSubOp(const MediumDecBase& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtractionOp(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (WasNegative)//-4.5 - 5.6
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; --IntValue; }
                }
                else//0.995 - 1
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += MediumDecBase::DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= MediumDecBase::DecimalOverflow) { DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = MediumDecBase::DecimalOverflow - DecimalHalf;
        }

	#pragma endregion NormalRep AltNum Subtraction Operations
	
    #pragma region Other Division Operations

        template<IntegerType IntType=signed int>
		void CatchAllIntDivision(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntDivOp(rValue);
		}

        template<IntegerType IntType=signed int>
		void CatchAllUIntDivision(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntDivOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllIntDivisionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntDivOp(rValue);
        }

        /// <summary>
        /// Division Operation Between MediumDecBase and Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<typename IntType = int>
        MediumDecBase& IntDivOp(const IntType& rValue)
        {
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                {
                    #if defined(AltNum_EnableAlternativeRepFractionals)
                    if (rValue < 0)
                    {
                        ExtraRep = -rValue;
                        SwapNegativeStatus();
                    }
                    else
                        ExtraRep = rValue;
                    #else
                    BasicIntDivOp(rValue);
                    #endif
                }
                break;
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep;
                    if (rValue < 0)
                    {
                        int invertedR = -rValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (ExtraRep == result / invertedR)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(invertedR);
                    }
                    else
                    {
                        result *= rValue;
                        if (ExtraRep == result / rValue)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(rValue);
                    }
                }
                break;
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = DecimalHalf;
                    if (rValue < 0)
                    {
                        int invertedR = -rValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (DecimalHalf == result / invertedR)//checking for overflow
                            DecimalHalf = result;
                        else
                            CatchAllUIntDivision(invertedR, LRep);
                    }
                    else
                    {
                        result *= rValue;
                        if (DecimalHalf == result / rValue)//checking for overflow
                            DecimalHalf = result;
                        else
                            CatchAllUIntDivision(rValue, LRep);
                    }
                }
                break;
    #endif
#endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep;
                    if (rValue < 0)
                    {
                        int invertedR = -rValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (ExtraRep == result / invertedR)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(invertedR);
                    }
                    else
                    {
                        result *= rValue;
                        if (ExtraRep == result / rValue)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(rValue);
                    }
                }
                break; 
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    if(rValue<0)
                        IntValue *= -1;
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                {
                    #if defined(AltNum_EnableDecimaledIFractionals)
                    if (rValue < 0)
                    {
                        ExtraRep = rValue;
                        SwapNegativeStatus();
                    }
                    else
                        ExtraRep = -rValue;
                    #elif defined(AltNum_EnableIFractional)
                    if (DecimalHalf == 0)
                    {
                        if (rValue < 0)
                        {
                            DecimalHalf = -rValue;
                            SwapNegativeStatus();
                        }
                        else
                            DecimalHalf = rValue;
                        ExtraRep = IByDivisorRep;
                    }
                    else
                        BasicUIntDivOp(rValue);
                    #else
                    BasicUIntDivOp(rValue);
                    #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                {
                    if (IsAtZeroInt())
                    {
                        if (rValue < 0)
                            SwapNegativeStatus();
                        return *this;
                    }
                    ConvertToNormType(LRep);
                    BasicIntDivOp(rValue);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                #endif
        #endif
                {
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    if(rValue<0)
                        IntValue *= -1;
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
            #endif
        #endif
                {
                    ConvertToNormalIRep(LRep);
                    BasicIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become NumByDiv
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            if (IntValue == NegativeRep)
                                IntValue = divRes;
                            else
                                IntValue = -divRes;
                            DecimalHalf = 0;
                        }
                        else
                        {
                            if (IntValue == NegativeRep)
                                IntValue = DecimalHalf;
                            else
                                IntValue = -DecimalHalf;
                            DecimalHalf = 0;
                            ExtraRep *= rValue;
                        }
                        return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Impliment code here";
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
                    }
                }
                break;
        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
        #endif
        #if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become Fractional
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        else
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        //return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Impliment code here";
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
                    }
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif			
                default:
                    BasicIntDivOp(rValue);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Division Operation Between MediumDecBase and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& UIntDivOp(const IntType& rValue)
        {
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                {
                    #if defined(AltNum_EnableAlternativeRepFractionals)
                    ExtraRep = rValue;
                    #else
                    BasicUIntDivOp(rValue);
                    #endif
                }
                break;
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                {
                    int result = ExtraRep * rValue;
                    if (ExtraRep == result / rValue)//checking for overflow
                        ExtraRep = result;
                    else
                        BasicUIntDivOp(rValue);
                }
                break;
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep * rValue;
                    if (DecimalHalf == result / rValue)//checking for overflow
                        DecimalHalf = result;
                    else
                        CatchAllIntDivision(rValue, LRep);
                }
                break;
    #endif
#endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    #endif
                {
                    int result = ExtraRep * rValue;
                    if (ExtraRep == result / rValue)//checking for overflow
                        ExtraRep = result;
                    else
                        BasicUIntDivOp(rValue);
                }
                break; 
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                {
                    #if defined(AltNum_EnableDecimaledIFractionals)
                    ExtraRep = -rValue;
                    #elif defined(AltNum_EnableIFractional)
                    if (DecimalHalf == 0)
                    {
                        DecimalHalf = rValue;
                        ExtraRep = IByDivisorRep;
                    }
                    else
                        BasicUIntDivOp(rValue);
                    #else
                    BasicUIntDivOp(rValue);
                    #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                {
                    if (IsAtZeroInt())
                        return *this;
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rValue);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                #endif
        #endif
                {
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
            #endif
        #endif
                {
                    ConvertToNormalIRep(LRep);
                    BasicIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become NumByDiv
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            if (IntValue == NegativeRep)
                                IntValue = divRes;
                            else
                                IntValue = -divRes;
                            DecimalHalf = 0;
                        }
                        else
                        {
                            if (IntValue == NegativeRep)
                                IntValue = DecimalHalf;
                            else
                                IntValue = -DecimalHalf;
                            DecimalHalf = 0;
                            ExtraRep *= rValue;
                        }
                        return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Impliment code here";
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
                    }
                }
                break;
        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
        #endif
        #if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become Fractional
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        else
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        //return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Impliment code here";
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
                    }
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif			
                default:
                    BasicIntDivOp(rValue);
                    break;
            }
            return *this;
        }
		
        /// <summary>
        /// Division Operation Between MediumDecBase and Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType = int>
        static MediumDecBase& IntDivision(MediumDecBase self, const IntType& rValue) { return self.IntDivOp(rValue); }

        template<typename IntType = int>
        MediumDecBase DivideByInt(const MediumDecBase& rValue) { MediumDecBase self = *this; return self.IntDivOp(rValue); }

        template<typename IntType = int>
        MediumDecBase DivideByUInt(const IntType& rValue) { MediumDecBase self = *this; return self.UIntDivOp(rValue); }

    #pragma endregion Other Division Operations

    #pragma region Other Multiplication Operations
public:

        template<IntegerType IntType=signed int>
		void CatchAllIntMultiplication(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntMultOp(rValue);
		}

        template<IntegerType IntType=signed int>
		void CatchAllUIntMultiplication(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntMultOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllIntMultiplicationV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntMultOp(rValue);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& IntMultOp(IntType rValue)
        {
            if (IsZero()||rValue==1)
                return *this;
            if (rValue == 0)
            {
                SetAsZero(); return *this;
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
					BasicIntMultOp(rValue);
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                {//Reduce divisor if possible
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break;
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {
					IntValue *= rValue;
					if(IntValue==0)
					{
						DecimalHalf = 0; ExtraRep = 0;
					}
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    #endif
                {
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break; 
    #endif
#endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
					BasicIntMultOp(rValue);
					break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
					if(IntValue==0||IntValue==NegativeRep)
					{
						if(rValue<0)
							IntValue = NegativeRep;
					}
					else
						CatchAllIntMultiplication(rValue, LRep);
					break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus;
					}
					if(IntValue==NegativeRep)
						IntValue = -(int)rValue;
					else if(IntValue==0)
						IntValue = (int)rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*(int)rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*(int)rValue - 1;
					break;
                #endif
        #if defined(AltNum_EnableApproachingDivided)
		        case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                {
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					if(IntValue==0||IntValue==NegativeRep)
					{
						if(rValue<0)
							IntValue = NegativeRep;
					}
					else
						CatchAllIntMultiplication(rValue, LRep);
					break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus;
					}
					if(IntValue==NegativeRep)
						IntValue = -rValue;
					else if(IntValue==0)
						IntValue = rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*rValue - 1;
					break;
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                {
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
				{
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else
					{
						ConvertToNormalIRep(LRep);
						BasicIntMultOp(rValue);
					}
				}
				break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
					DecimalHalf *= rValue;
					IntHalfMultiplicationOp(rValue);
					int divRes = DecimalHalf / -ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf + increment;
                    }
				}
                break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
		#endif
		#if defined(AltNum_EnableAlternativeMixedFrac)
                {
					if(rValue<0)
					{
						rValue *= -1;
						SwapNegativeStatus();
					}
                    DecimalHalf *= rValue;
					IntHalfMultiplicationOp(rValue);
                    int divRes = DecimalHalf / ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf - increment;
                    }
                    break;
                }
		#endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif			
                default:
                    CatchAllIntMultiplication(rValue, LRep);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& UIntMultOp(const IntType& rValue)
        {
            if (IsZero()||rValue==1)
                return *this;
            if (rValue == 0)
            {
                SetAsZero(); return *this;
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
					BasicUIntMultOp(rValue);
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                {//Reduce divisor if possible
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break;
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {
					IntValue *= rValue;
					if(IntValue==0)
					{
						DecimalHalf = 0; ExtraRep = 0;
					}
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    #endif
                {
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break; 
    #endif
#endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
					BasicUIntMultOp(rValue);
					break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
					if(IntValue!=0&&IntValue!=NegativeRep)
						CatchAllUIntMultiplication(rValue, LRep);
					break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
					if(IntValue==NegativeRep)
						IntValue = -(int)rValue;
					else if(IntValue==0)
						IntValue = (int)rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*(int)rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*(int)rValue - 1;
					break;
                #endif
        #if defined(AltNum_EnableApproachingDivided)
		        case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                {
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
							{
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					if(IntValue==0||IntValue==NegativeRep)
					{
						if(rValue<0)
							IntValue = NegativeRep;
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
					break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					if(IntValue==NegativeRep)
						IntValue = -(int)rValue;
					else if(IntValue==0)
						IntValue = (int)rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*(int)rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*(int)rValue - 1;
					break;
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                {
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
				{
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else
					{
						ConvertToNormalIRep(LRep);
						BasicUIntMultOp(rValue);
					}
				}
				break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
					DecimalHalf *= rValue;
					UIntHalfMultiplicationOp(rValue);
					int divRes = DecimalHalf / -ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf + increment;
                    }
				}
                break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
		#endif
		#if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    DecimalHalf *= rValue;
					UIntHalfMultiplicationOp(rValue);
                    int divRes = DecimalHalf / ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf - increment;
                    }
                }
				break;
		#endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif			
                default:
                    CatchAllUIntMultiplication(rValue, LRep);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType = int>
        static MediumDecBase& IntMultiplication(MediumDecBase self, const IntType& rValue) { return self.IntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and unsigned Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType = int>
        static MediumDecBase& UIntMultiplication(MediumDecBase self, const IntType& rValue) { return self.UIntMultOp(rValue); }


        template<typename IntType = int>
        MediumDecBase MultiplyByInt(const MediumDecBase& rValue) { MediumDecBase self = *this; return self.IntMultOp(rValue); }

        template<typename IntType = int>
        MediumDecBase MultipleByUInt(const IntType& rValue) { MediumDecBase self = *this; return self.UIntMultOp(rValue); }

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations
public:

        template<IntegerType IntType=signed int>
		void CatchAllIntAddition(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntAddOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllIntAdditionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntAddOp(rValue);
        }

        /// <summary>
        /// Addition MediumDecBase modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& IntAddOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert MediumDecBase into complex number at moment";
				return *this;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAdditionOp(rValue);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
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
            return *this;
        }

        /// <summary>
        /// Addition Operation Between MediumDecBase and Integer Value.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType = int>
        static MediumDecBase& IntAddition(MediumDecBase self, const IntType& rValue) { return self.IntAddOp(rValue); }

        template<typename IntType = int>
        MediumDecBase AdditionByInt(const MediumDecBase& rValue) { MediumDecBase self = *this; return self.IntAddOp(rValue); }

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

        template<IntegerType IntType=signed int>
		void CatchAllIntSubtraction(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntSubOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllIntSubtractionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntSubOp(rValue);
        }

        /// <summary>
        /// Subtraction MediumDecBase modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType=signed int>
        MediumDecBase& IntSubOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert MediumDecBase into complex number at moment";
				return *this;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAdditionOp(rValue);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
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
            return *this;
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer Value.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>MediumDecBase</returns>
        template<typename IntType = int>
        static MediumDecBase& IntSubtraction(MediumDecBase self, const IntType& rValue) { return self.IntSubOp(rValue); }

        template<typename IntType = int>
        MediumDecBase SubtractionByInt(const MediumDecBase& rValue) { MediumDecBase self = *this; return self.IntSubOp(rValue); }

    #pragma endregion Other Subtraction Operations

	#pragma region Main AltNum Operations
public:

		void CatchAllDivision(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicMultOp(convertedRVal);
		}

        MediumDecBase CatchAllDivisionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllDivision(rValue, LRep, RRep); return self; }
		
		void CatchAllDivisionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicUnsignedDivOp(convertedRVal);
		}

        MediumDecBase CatchAllDivisionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllDivisionV2(rValue, SameRep); return self; }
	
	   void CatchAllDivisionV3(MediumDecBase& rValue)
	   {
			ConvertToNormTypeV2();
            MediumDecBase convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicUnsignedDivOp(convertedRVal);
	   }

        MediumDecBase CatchAllDivisionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllDivisionV3(rValue); return self; }
		
	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryDivision(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
		   ConvertIRepToNormal(LRep);
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(RRep);
		   BasicUnsignedDivOp(convertedRVal);
		   ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginaryDivisionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllImaginaryDivision(rValue, LRep, RRep); return self; }
		
		void CatchAllImaginaryDivisionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertIRepToNormal(SameRep);
            MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicUnsignedDivOp(convertedRVal);
            ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginaryDivisionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllImaginaryDivisionV2(rValue, SameRep); return self; }
	
        void CatchAllImaginaryDivisionV3(MediumDecBase& rValue)
        {
           ConvertIRepToNormalV2();
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicUnsignedDivOp(convertedRVal);
           ExtraRep = 0;
        }

        MediumDecBase CatchAllImaginaryDivisionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllImaginaryDivisionV3(rValue); return self; }
	#endif

public:
        static void RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecBase& self, MediumDecBase Value);

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="Value">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& DivOp(MediumDecBase rValue)
        {
            //Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
            if (IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (rValue.DecimalHalf == InfinityRep)
            {
                if (DecimalHalf == InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
    #if defined(AltNum_EnableNaN)
                    DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
    #else
                    throw "Result is undefined(Infinity by Infinity)";
    #endif
                else
    #if defined(AltNum_EnableApproachingValues)
                {
                    DecimalHalf = ApproachingBottomRep;
                    if(rValue.IntValue<0)
                    {
                        if (IntValue<0)
                            IntValue = 0;
                        else
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        if (IntValue<0)
                            IntValue = NegativeRep;
                        else
                            IntValue = 0;
                    }
                    ExtraRep = 0;
                }
    #else            
                    SetAsZero();
    #endif
                return *this;
            }
            if (rValue.IntValue==0&&rValue.DecimalHalf==0)
            {
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
            }
#else
            if (rValue.IsZero())
                throw "Target value can not be divided by zero unless infinity enabled";
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedDivOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
#endif
#if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableENum) || defined(AltNum_EnableImaginaryNum)
                    ExtraRep = 0;
                    BasicUnsignedDivOp(rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rValue);
                    if (rValue.ExtraRep<ExtraRep)
                    {
                        MediumDecBase PiPowerDivisor = PiPowerNum(ExtraRep - rValue.ExtraRep);
                        ExtraRep = 0;
                        BasicUnsignedDivOp(PiPowerDivisor);
                    }
                    else
                    {
                        ExtraRep -= rValue.ExtraRep;
                    }
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    int rInverted = rValue.IntValue==NegativeRep?0:-rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == 0 && rValue.IntValue == NegativeRep)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = rValue.IntValue==NegativeRep?0:-rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1; DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;

                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    if (ExtraRep == rValue.ExtraRep)
                    {// 0.249..9 / 0.249..9 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == NegativeRep && rValue.IntValue == 0)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                case RepType::ApproachingMidRight:
                    if (ExtraRep == rValue.ExtraRep)
                    {//0.50..1 / 0.50..1 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(MediumDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((MediumDecBase(IntValue,DecimalHalf))/ExtraRep) / (MediumDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((MediumDecBase(IntValue,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(MediumDecBase(rValue.IntValue,rValue.DecimalHalf)))
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {//CatchAllDivisionV2(rValue, LRep);
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
                //(Self.IntValue/DecimalHalf)/(rValue.IntValue/rValue.DecimalHalf) =
                //(IntValue*rValue.DecimalHalf)/(DecimalHalf*rValue.IntValue)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
    #if defined(AltNum_UsingAltFractional)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int NumRes = rValue.DecimalHalf * IntValue;
                    int DenomRes = DecimalHalf * rValue.IntValue;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableDecimaledAlternativeFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #endif
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif

                //Turn MixedFrac into fractional and then apply			
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    MediumDecBase LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = MediumDecBase(DecimalHalf);
                    else if (IntValue < 0)
                        LeftSideNum = MediumDecBase(IntValue * ExtraRep + DecimalHalf);
                    else if (IntValue == 0)
                        LeftSideNum = MediumDecBase(-DecimalHalf);
                    else
                        LeftSideNum = MediumDecBase(IntValue * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==NegativeRep ? -rValue.DecimalHalf : -rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
    #endif
    #if defined(AltNum_MixedAltEnabled)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //=LeftSideNum*-rValue.ExtraRep / RightSideNum;
                    int rvDivisor = -rValue.ExtraRep;
                    MediumDecBase LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntValue==0?-DecimalHalf:(rValue.IntValue*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == NegativeRep ? DecimalHalf : (-rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == 0 ? DecimalHalf : (rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                    {
                        if (DecimalHalf == InfinityRep)
                            SetVal(One);
                        else
                            #if defined(AltNum_EnableNaN)
                            DecimalHalf = UndefinedRep;
                        #else
                            throw "Undefined result(results in undefined expression)";
                        #endif
                    }
                    else
                        BasicUnsignedDivOp(rValue);
                }
                break;
    #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
    #endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType division not supported yet";
                    break;
                }
            }
            else
                RepToRepDivOp(LRep, RRep, *this, rValue);
            return *this;
        }
   
   MediumDecBase DivideBy(const MediumDecBase& rValue) { MediumDecBase self = *this; self.DivOp(rValue); return self; }

        /// <summary>
        /// Division Operation with only positive right side values
        /// </summary>
        /// <param name="Value">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& UnsignedDivOp(const MediumDecBase& rValue)
        {
            //Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
            if (IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (rValue.DecimalHalf == InfinityRep)
            {
                if (DecimalHalf == InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
    #if defined(AltNum_EnableNaN)
                    DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
    #else
                    throw "Result is undefined(Infinity by Infinity)";
    #endif
                else
    #if defined(AltNum_EnableApproachingValues)
                {
                    DecimalHalf = ApproachingBottomRep;
                    if(rValue.IntValue<0)
                    {
                        if (IntValue<0)
                            IntValue = 0;
                        else
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        if (IntValue<0)
                            IntValue = NegativeRep;
                        else
                            IntValue = 0;
                    }
                    ExtraRep = 0;
                }
    #else            
                    SetAsZero();
    #endif
                return *this;
            }
            if (rValue.IntValue==0&&rValue.DecimalHalf==0)
            {
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
            }
#else
            if (rValue.IsZero())
                throw "Target value can not be divided by zero unless infinity enabled";
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedDivOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
#endif
#if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableENum) || defined(AltNum_EnableImaginaryNum)
                    ExtraRep = 0;
                    BasicUnsignedDivOp(rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rValue);
                    if (rValue.ExtraRep<ExtraRep)
                    {
                        MediumDecBase PiPowerDivisor = PiPowerNum(ExtraRep - rValue.ExtraRep);
                        ExtraRep = 0;
                        BasicUnsignedDivOp(PiPowerDivisor);
                    }
                    else
                    {
                        ExtraRep -= rValue.ExtraRep;
                    }
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    int rInverted = -rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == 0 && rValue.IntValue == NegativeRep)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = -rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1; DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;

                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    if (ExtraRep == rValue.ExtraRep)
                    {// 0.249..9 / 0.249..9 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == NegativeRep && rValue.IntValue == 0)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                case RepType::ApproachingMidRight:
                    if (ExtraRep == rValue.ExtraRep)
                    {//0.50..1 / 0.50..1 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(MediumDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((MediumDecBase(IntValue,DecimalHalf))/ExtraRep) / (MediumDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((MediumDecBase(IntValue,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(MediumDecBase(rValue.IntValue,rValue.DecimalHalf)))
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
                //(Self.IntValue/DecimalHalf)/(rValue.IntValue/rValue.DecimalHalf) =
                //(IntValue*rValue.DecimalHalf)/(DecimalHalf*rValue.IntValue)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
    #if defined(AltNum_UsingAltFractional)
                {
                    int NumRes = rValue.DecimalHalf * IntValue;
                    int DenomRes = DecimalHalf * rValue.IntValue;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableDecimaledAlternativeFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #endif
                {
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif

                //Turn MixedFrac into fractional and then apply			
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    MediumDecBase LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = MediumDecBase(DecimalHalf);
                    else if (IntValue < 0)
                        LeftSideNum = MediumDecBase(IntValue * ExtraRep + DecimalHalf);
                    else if (IntValue == 0)
                        LeftSideNum = MediumDecBase(-DecimalHalf);
                    else
                        LeftSideNum = MediumDecBase(IntValue * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==NegativeRep ? -rValue.DecimalHalf : -rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
    #endif
    #if defined(AltNum_MixedAltEnabled)
                {
                    //=LeftSideNum*-rValue.ExtraRep / RightSideNum;
                    int rvDivisor = -rValue.ExtraRep;
                    MediumDecBase LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntValue==0?-DecimalHalf:(rValue.IntValue*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == NegativeRep ? DecimalHalf : (-rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == 0 ? DecimalHalf : (rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                    {
                        if (DecimalHalf == InfinityRep)
                            SetVal(One);
                        else
                            #if defined(AltNum_EnableNaN)
                            DecimalHalf = UndefinedRep;
                        #else
                            throw "Undefined result(results in undefined expression)";
                        #endif
                    }
                    else
                        BasicUnsignedDivOp(rValue);
                }
                break;
    #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
    #endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType division not supported yet";
                    break;
                }
            }
            else
                RepToRepDivOp(LRep, RRep, *this, rValue);
            return *this;
        }

        MediumDecBase DivideByUnsigned(const MediumDecBase& rValue) { MediumDecBase self = *this; self.UnsignedDivOp(rValue); return self; }

		void CatchAllMultiplication(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicMultOp(convertedRVal);
		}
		
		void CatchAllMultiplicationV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicMultOp(convertedRVal);
		}
		
		void CatchAllMultiplicationV3(MediumDecBase& rValue)
		{
			ConvertToNormTypeV2();
            MediumDecBase convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicMultOp(convertedRVal);
		}

	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryMultiplication(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
		   ConvertIRepToNormal(LRep);
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(RRep);
		   BasicMultOp(convertedRVal);
		   ExtraRep = 0;
		   SwapNegativeStatus();
		}

        MediumDecBase CatchAllImaginaryMultiplicationAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllImaginaryMultiplication(rValue, LRep, RRep); return self; }
		
		void CatchAllImaginaryMultiplicationV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertIRepToNormal(SameRep);
            MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicMultOp(convertedRVal);
            ExtraRep = 0;
            SwapNegativeStatus();
		}

        MediumDecBase CatchAllImaginaryMultiplicationAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep) 
        { MediumDecBase self = *this; CatchAllImaginaryMultiplicationV2(rValue, SameRep); return self; }
	
	   void CatchAllImaginaryMultiplicationV3(MediumDecBase& rValue)
	   {
           ConvertIRepToNormalV2();
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicMultOp(convertedRVal);
           ExtraRep = 0;
           SwapNegativeStatus();
	   }

        MediumDecBase CatchAllImaginaryMultiplicationAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllImaginaryMultiplicationV3(rValue); return self; }
	#endif

        static void RepToRepMultOp(RepType& LRep, RepType& RRep, MediumDecBase& self, MediumDecBase rValue);

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& MultOp(MediumDecBase rValue)
        {
            if (rValue.IntValue==0&&rValue.DecimalHalf==0) { SetAsZero(); return *this; }
            if (IsZero() || rValue == MediumDecBase::One)
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rValue<0)
                    IntValue *= -1;
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicMultOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    BasicMultOp(rValue);
#if defined(AltNum_EnablePiPowers)
                    ExtraRep = -2;
#else
                    BasicMultOp(PiNum);
#endif
                    break;
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
                    BasicMultOp(rValue);
                    BasicMultOp(ENum);
                    break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi * Yi = -XY
                    ExtraRep = 0;
                    BasicMultOp(-rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ExtraRep += rValue.ExtraRep;
                    BasicMultOp(rValue);
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    if (IntValue == NegativeRep)
                    {
                        if(rValue<0)
                            IntValue = 0;
                    }
                    else if (IntValue == 0)
                    {
                        if(rValue<0)
                            IntValue = NegativeRep;
                    }
                    else if (IntValue < 0)
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntValue == NegativeRep)//-1.0..1 * -0.0..1
                                IntValue = 0;
                            else//-1.0..1 * -2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        else
                        {
                            if (rValue.IntValue == 0)//-1.0..1 * 0.0..1
                                IntValue = NegativeRep;
                            else//-1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                    }
                    else
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntValue == NegativeRep)//1.0..1 * -0.0..1
                                IntValue = NegativeRep;
                            else//1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        else
                        {
                            if (rValue.IntValue == 0)//1.0..1 * 0.0..1
                                IntValue = 0;
                            else//1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                    }
                    return *this;
                }
                break;
                case RepType::ApproachingTop://Just going to convert into normal numbers for now
                {
                    CatchAllMultiplicationV2(rValue, LRep);
                }    
                break;

    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                {
                    if(rValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    if (rValue.IntValue == 0)
                    {
                        //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        //0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:2)
                        // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        // 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:16)
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntValue;
                        int XZ = IntValue * InvertedrValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                    break;
                }
                case RepType::ApproachingMidRight:
                {
                    if(rValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0)
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if (rValue.IntValue == 0)
                    {
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue.IntValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntValue;
                        int XZ = IntValue * InvertedrValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                }
                break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(MediumDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((MediumDecBase(IntValue,DecimalHalf))/ExtraRep) * (MediumDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((MediumDecBase(IntValue,DecimalHalf))*MediumDecBase(rValue.IntValue,rValue.DecimalHalf)))/(ExtraRep*rValue.ExtraRep)
                    BasicMultOp(rValue);
                    ExtraRep *= rValue.ExtraRep;
                }
                break;

                //(IntValue*rValue.IntValue)*Pi^2/(DecimalHalf*rValue.DecimalHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {//Become either PiNum, NumByDiv, or PiPower
        #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                    IntValue *= rValue.IntValue;
                    int divisor = DecimalHalf*rValue.DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDiv(divisor);
                    ExtraRep = -2;//Pi^2
        #else//Becoming PiNum
                    IntValue *= rValue.IntValue;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(PiNum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = PiRep;
        #endif         
                }
                break;
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    IntValue *= rValue.IntValue;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(ENum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
                break;
    #endif

    #if defined(AltNum_EnableImaginaryNum)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    //(IntValue/DecimalHalf)i*(rValue.IntValue/rValue.DecimalHalf)i
                    //==-1*(IntValue/DecimalHalf)*(rValue.IntValue/rValue.DecimalHalf)
                    int NumRes = -IntValue * rValue.IntValue;
                    int DenomRes = DecimalHalf * rValue.DecimalHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif

    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/-ExtraRep)*Pi Representation
                    //(rValue/(-ExtraRep))*Pi * (rValue.rValue/(-rValue.ExtraRep))*Pi
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/-ExtraRep)*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/-ExtraRep)*i Representation
                    //(rValue/(-ExtraRep))*i * (rValue.rValue/(-rValue.ExtraRep))*i
        #endif
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    ExtraRep *= -rValue.ExtraRep;
                    BasicUnsignedMultOp(rValue);
            #if defined(AltNum_EnableDecimaledPiFractionals)
                #ifdef AltNum_EnablePiPowers//Convert to PiPower representation
                    BasicUnsignedDivOp(-ExtraRep);
                    ExtraRep = -2;
                #else
                    BasicUnsignedMultOp(PiNum);
                #endif
            #elif defined(AltNum_EnableDecimaledEFractionals)
                    BasicUnsignedMultOp(ENum);
            #else
                    SwapNegativeStatus();
            #endif
                }
                break;
    #endif
#endif

                    //Turn MixedFrac into fractional and then apply
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //IntValue +- (-DecimalHalf/ExtraRep) = 
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;

                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes PiNumByDiv
                    //And then multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #elif defined(AltNum_EnablePiPowers)
                    //Or convert to PiPower (of 2)
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #else
                    //Or convert PiNum and multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;

#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes ENumByDiv
                    //And then multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #else
                    //Or convert ENum and multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = -DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                        DecimalHalf = InfinityRep;
                    else
                        BasicMultOp(rValue);
                }
                break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
#endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType multiplication not supported yet";
                    break;
                }
            }
            else
                RepToRepMultOp(LRep, RRep, *this, rValue);
            return *this;
        }
		
        MediumDecBase MultipleBy(const MediumDecBase& rValue) { MediumDecBase self = *this; self.MultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& UnsignedMultOp(const MediumDecBase& rValue)
        {
            if (rValue.IntValue==0&&rValue.DecimalHalf==0) { SetAsZero(); return *this; }
            if (IsZero() || rValue == MediumDecBase::One)
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rValue<0)
                    IntValue *= -1;
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedMultOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    BasicUnsignedMultOp(rValue);
#if defined(AltNum_EnablePiPowers)
                    ExtraRep = -2;
#else
                    BasicUnsignedMultOp(PiNum);
#endif
                    break;
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
                    BasicUnsignedMultOp(rValue);
                    BasicUnsignedMultOp(ENum);
                    break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi * Yi = -XY
                    ExtraRep = 0;
                    BasicUnsignedMultOp(-rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ExtraRep += rValue.ExtraRep;
                    BasicUnsignedMultOp(rValue);
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 * 0.0..1
                            IntValue = NegativeRep;
                        else//-1.0..1 * 2.0..1
                            IntValue *= rValue.IntValue;
                    }
                    else if (IntValue != NegativeRep||IntValue!=0)
                    {
                        if (rValue.IntValue == 0)//1.0..1 * 0.0..1
                            IntValue = 0;
                        else//1.0..1 * 2.0..1
                            IntValue *= rValue.IntValue;
                    }
                    return *this;
                }
                break;
                case RepType::ApproachingTop://Just going to convert into normal numbers for now
                {
                    CatchAllMultiplicationV2(rValue, LRep);
                }    
                break;

    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                {
                    if (rValue.IntValue == 0)
                    {
                        //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        //0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:2)
                        // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        // 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:16)
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                    break;
                }
                case RepType::ApproachingMidRight:
                {
                    if (rValue.IntValue == 0)
                    {
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        MediumDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        MediumDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        MediumDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                }
                break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(MediumDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((MediumDecBase(IntValue,DecimalHalf))/ExtraRep) * (MediumDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((MediumDecBase(IntValue,DecimalHalf))*MediumDecBase(rValue.IntValue,rValue.DecimalHalf)))/(ExtraRep*rValue.ExtraRep)
                    BasicUnsignedMultOp(rValue);
                    ExtraRep *= rValue.ExtraRep;
                }
                break;

                //(IntValue*rValue.IntValue)*Pi^2/(DecimalHalf/rValue.DecimalHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {
        #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                    IntValue *= rValue.IntValue;
                    int divisor = DecimalHalf*rValue.DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDiv(divisor);
                    ExtraRep = -2;//Pi^2
        #else//Becoming PiNum
                    IntValue *= rValue.IntValue;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(PiNum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = PiRep;
        #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    IntValue *= rValue.IntValue;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(ENum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
                break;
    #endif

    #if defined(AltNum_EnableImaginaryNum)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    //(IntValue/DecimalHalf)i*(rValue.IntValue/rValue.DecimalHalf)i
                    //==-1*(IntValue/DecimalHalf)*(rValue.IntValue/rValue.DecimalHalf)
                    int NumRes = -IntValue * rValue.IntValue;
                    int DenomRes = DecimalHalf * rValue.DecimalHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif

    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/-ExtraRep)*Pi Representation
                    //(rValue/(-ExtraRep))*Pi * (rValue.rValue/(-rValue.ExtraRep))*Pi
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/-ExtraRep)*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/-ExtraRep)*i Representation
                    //(rValue/(-ExtraRep))*i * (rValue.rValue/(-rValue.ExtraRep))*i
        #endif
                {
                    ExtraRep *= -rValue.ExtraRep;
                    BasicUnsignedMultOp(rValue);
            #if defined(AltNum_EnableDecimaledPiFractionals)
                #ifdef AltNum_EnablePiPowers//Convert to PiPower representation
                    BasicUnsignedDivOp(-ExtraRep);
                    ExtraRep = -2;
                #else
                    BasicUnsignedMultOp(PiNum);
                #endif
            #elif defined(AltNum_EnableDecimaledEFractionals)
                    BasicUnsignedMultOp(ENum);
            #else
                    SwapNegativeStatus();
            #endif
                }
                break;
    #endif
#endif

                    //Turn MixedFrac into fractional and then apply
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    //IntValue +- (-DecimalHalf/ExtraRep) = 
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                {
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;

                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes PiNumByDiv
                    //And then multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #elif defined(AltNum_EnablePiPowers)
                    //Or convert to PiPower (of 2)
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #else
                    //Or convert PiNum and multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;

#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                {
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes ENumByDiv
                    //And then multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #else
                    //Or convert ENum and multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                {
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = -DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                        DecimalHalf = InfinityRep;
                    else
                        BasicUnsignedMultOp(rValue);
                }
                break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
#endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType multiplication not supported yet";
                    break;
                }
            }
            else
                RepToRepMultOp(LRep, RRep, *this, rValue);
            return *this;
        }

        MediumDecBase MultipleByUnsigned(const MediumDecBase& rValue) { MediumDecBase self = *this; self.UnsignedMultOp(rValue); return self; }

		void CatchAllAddition(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicMultOp(convertedRVal);
		}

        MediumDecBase CatchAllAdditionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllAddition(rValue, LRep, RRep); return self; }
		
		void CatchAllAdditionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicAddOp(convertedRVal);
		}

        MediumDecBase CatchAllAdditionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllAdditionV2(rValue, SameRep); return self; }
	
	   void CatchAllAdditionV3(MediumDecBase& rValue)
	   {
			ConvertToNormTypeV2();
            MediumDecBase convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicAddOp(convertedRVal);
	   }

        MediumDecBase CatchAllAdditionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllAdditionV3(rValue); return self; }
		
	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryAddition(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
		   ConvertIRepToNormal(LRep);
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(RRep);
		   BasicAddOp(convertedRVal);
		   ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginaryAdditionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllImaginaryAddition(rValue, LRep, RRep); return self; }
		
		void CatchAllImaginaryAdditionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertIRepToNormal(SameRep);
            MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicAddOp(convertedRVal);
            ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginaryAdditionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllImaginaryAdditionV2(rValue, SameRep); return self; }
	
        void CatchAllImaginaryAdditionV3(MediumDecBase& rValue)
        {
           ConvertIRepToNormalV2();
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicAddOp(convertedRVal);
           ExtraRep = 0;
        }

        MediumDecBase CatchAllImaginaryAdditionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllImaginaryAdditionV3(rValue); return self; }
	#endif

        static void RepToRepAddOp(RepType& LRep, RepType& RRep, MediumDecBase& self, MediumDecBase rValue);

        /// <summary>
        /// Addition Operation  with right side MediumDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& AddOp(MediumDecBase rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
#endif
                    BasicAddOp(rValue);
                    break;

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 + 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 - 0.0..1 = -0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue < 0)//-0.0..1 - 1.0..1 = -1.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                        else//-0.0..1 + 5.0..1 = 5
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 + 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 - 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.0..1 - 1.0..1 = -1
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.0..1 + 5.0..1 = 5.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 + 0.0..1  = -1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 - 0.0..1 = -1.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 - 2.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.0..1 + 2.0..1 = 1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.0..1 + 1.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 + 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
                            IntValue = -1;
                        else if (rValue.IntValue < 0)//-0.9..9 - 1.9..9 = -2.9..8
                        {
                            IntValue = rValue.IntValue - 1;
                        }
                        else//-0.9..9 + 5.9..9 = 5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue == NegativeRep)//0.9..9 - 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.9..9 - 1.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.9..9 + 5.9..9 = 6.9..8
                        {
                            IntValue = rValue.IntValue + 1;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 + 0.9..9  = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.9..9 - 0.9..9 = -2.9..9
                            IntValue += rValue.IntValue;
                        else if (IntValue == -rValue.IntValue)//-1.9..9 + 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 - 2.9..9
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 + 0.9..9 = 2.9..8
                        {
                            if (IntValue == -1)
                                IntValue = NegativeRep;
                            else
                                ++IntValue;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 - 0.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.9..9 + 1.9..9 = 3.9..8
                        {
                            IntValue += rValue.IntValue + 1;
                        }
                    }
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidRight);
                    break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
#if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnablePiFractional)||defined(AltNum_EnableEFractional)||defined(AltNum_EnableIFractional)
                    if (DecimalHalf == rValue.DecimalHalf)
                    {

                        *this += rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
#endif

#if defined(AltNum_EnableDecimaledAlternativeFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
#elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
#endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicAddOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicAddOp(rValue);
                    break;
#endif    

#if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue += rValue.IntValue * ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
#endif
#if defined(AltNum_EnableAlternativeMixedFrac)
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue += rValue.IntValue * -ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicAddOp(rValue);
                    break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        //Do nothing?
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        IntValue = NegativeRep;
                    }
                    else
                        IntValue += rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        //Do Nothing?
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// + positive infinity to right side
                        DecimalHalf = InfinityRep;
                    }
                    else
                        DecimalHalf += rValue.DecimalHalf;
                    break;
#endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType addition not supported yet";
                    break;
                }
            }
            else
                RepToRepAddOp(LRep, RRep, *this, rValue);
            return *this;
        }

        MediumDecBase AddBy(const MediumDecBase& rValue) { MediumDecBase self = *this; self.AddOp(rValue); return self; }

		void CatchAllSubtraction(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicMultOp(convertedRVal);
		}

        MediumDecBase CatchAllSubtractionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllSubtraction(rValue, LRep, RRep); return self; }
		
		void CatchAllSubtractionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicSubOp(convertedRVal);
		}

        MediumDecBase CatchAllSubtractionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllSubtractionV2(rValue, SameRep); return self; }
	
	   void CatchAllSubtractionV3(MediumDecBase& rValue)
	   {
			ConvertToNormTypeV2();
            MediumDecBase convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicSubOp(convertedRVal);
	   }

        MediumDecBase CatchAllSubtractionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllSubtractionV3(rValue); return self; }
		
	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginarySubtraction(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
		   ConvertIRepToNormal(LRep);
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(RRep);
		   BasicSubOp(convertedRVal);
		   ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginarySubtractionAsCopies(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
        { MediumDecBase self = *this; CatchAllImaginarySubtraction(rValue, LRep, RRep); return self; }
		
		void CatchAllImaginarySubtractionV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertIRepToNormal(SameRep);
            MediumDecBase convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicSubOp(convertedRVal);
            ExtraRep = 0;
		}

        MediumDecBase CatchAllImaginarySubtractionAsCopiesV2(MediumDecBase& rValue, const RepType& SameRep)
        { MediumDecBase self = *this; CatchAllImaginarySubtractionV2(rValue, SameRep); return self; }
	
        void CatchAllImaginarySubtractionV3(MediumDecBase& rValue)
        {
           ConvertIRepToNormalV2();
           MediumDecBase convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicSubOp(convertedRVal);
           ExtraRep = 0;
        }

        MediumDecBase CatchAllImaginarySubtractionAsCopiesV3(MediumDecBase& rValue)
        { MediumDecBase self = *this; CatchAllImaginarySubtractionV3(rValue); return self; }
	#endif

        static void RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecBase& self, MediumDecBase rValue);

        /// <summary>
        /// Subtraction Operation  with right side MediumDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& SubOp(MediumDecBase rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
#endif
                    BasicSubOp(rValue);
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 - 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 + 0.0..1 
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.0..1 + 1.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                        else//-0.0..1 - 5.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 - 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 + 0.0..1 = 0

                        else if (rValue.IntValue < 0)//0.0..1 + 1.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                        else//0.0..1 - 5.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 - 0.0..1  = -1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 + 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == rValue.IntValue)//-1.01 - 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 + 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.0..1 - 2.0..1 = 1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.0..1 - 1.0..1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 - 0.9..9 = 0
                            IntValue = -1;
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 + 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.9..9 + 1.9..9

                        else//-0.9..9 - 5.9..9 = -6.9..8
                            IntValue = -rValue.IntValue - 1;
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 - 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue < 0)//0.9..9 + 1.9..9 = 1.9..8
                            IntValue = -rValue.IntValue;
                        else//0.9..9 - 5.9..9 = -5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 - 0.9..9  = -2.9..8
                            --IntValue;
                        else if (rValue.IntValue == NegativeRep)//-1.9..9  + 0.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.9..9 - 2.9..9
                        {
                            IntValue -= rValue.IntValue + 1;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 - 0.9..9 = 2.9..8
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 + 0.9..9
                            ++IntValue;
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.9..9 - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidRight);
                    break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                        CatchAllSubtractionV2(rValue, RepType::NumByDiv);
                    break;
#if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_UsingAltFractional)
                    if (DecimalHalf == rValue.DecimalHalf)
                    {
                        *this -= rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
#endif

#if defined(AltNum_EnableDecimaledAlternativeFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
#elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
#endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicSubOp(rValue);
                    break;
#endif

#if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue -= rValue.IntValue * ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
#endif
#if defined(AltNum_EnableAlternativeMixedFrac)
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue -= rValue.IntValue * -ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicSubOp(rValue);
                    break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == NegativeRep)
                            IntValue = 0;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {//+positive infinity to left side
#if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
#else
                        throw "Uncertain result";
#endif
                    }
                    else
                        IntValue -= rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        if (rValue.DecimalHalf == InfinityRep)
                            DecimalHalf = 0;
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// subtractive infinity from real number
#if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
#else
                        throw "Uncertain result";
#endif
                    }
                    else
                        DecimalHalf -= rValue.DecimalHalf;
                    break;
#endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw MediumDecBase::RepTypeAsString(LRep) + " RepType subtraction not supported yet";
                    break;
                }
            }
            else
                RepToRepSubOp(LRep, RRep, *this, rValue);
            return *this;
        }

        MediumDecBase SubtractBy(const MediumDecBase& rValue) { MediumDecBase self = *this; self.SubOp(rValue); return self; }

	#pragma endregion Main AltNum Operations

    #pragma region Main Operator Overrides

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator/=(const MediumDecBase& rValue) { return DivOp(rValue); }

        /// <summary>
        /// Division Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator/=(const signed int& rValue) { return IntDivOp(rValue); }
        MediumDecBase& operator/=(const unsigned int& rValue) { return UIntDivOp(rValue); }
        MediumDecBase& operator/=(const unsigned __int64& rValue) { return UIntDivOp(rValue); }
        MediumDecBase& operator/=(const signed __int64& rValue) { return IntDivOp(rValue); }
        MediumDecBase& operator/=(const signed __int16& rValue) { return IntDivOp(rValue); }
        MediumDecBase& operator/=(const signed __int8& rValue) { return IntDivOp(rValue); }
        MediumDecBase& operator/=(const unsigned __int16& rValue) { return UIntDivOp(rValue); }
        MediumDecBase& operator/=(const unsigned __int8& rValue) { return UIntDivOp(rValue); }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase self, const MediumDecBase& rValue) { return self.DivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const int& rValue) { return self.IntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const unsigned int rValue) { return self.UIntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const signed long long rValue) { return self.IntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const unsigned __int64 rValue) { return self.UIntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const __int16& rValue) { return self.IntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const unsigned __int16& rValue) { return self.UIntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const signed __int8& rValue) { return self.IntDivOp(rValue); }
        friend MediumDecBase operator/(MediumDecBase self, const unsigned __int64& rValue) { return self.UIntDivOp(rValue); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator*=(const MediumDecBase& rValue) { return MultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator*=(const signed int& rValue) { return IntMultOp(rValue); }
        MediumDecBase& operator*=(const unsigned int& rValue) { return UIntMultOp(rValue); }
        MediumDecBase& operator*=(const unsigned __int64& rValue) { return UIntMultOp(rValue); }
        MediumDecBase& operator*=(const signed __int64& rValue) { return IntMultOp(rValue); }
        MediumDecBase& operator*=(const signed __int16& rValue) { return IntMultOp(rValue); }
        MediumDecBase& operator*=(const signed __int8& rValue) { return IntMultOp(rValue); }
        MediumDecBase& operator*=(const unsigned __int16& rValue) { return UIntMultOp(rValue); }
        MediumDecBase& operator*=(const unsigned __int8& rValue) { return UIntMultOp(rValue); }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(MediumDecBase self, const MediumDecBase& rValue) { return self.MultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const int& rValue) { return self.IntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const unsigned int rValue) { return self.UIntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const signed long long rValue) { return self.IntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const unsigned __int64 rValue) { return self.UIntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const __int16& rValue) { return self.IntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const unsigned __int16& rValue) { return self.UIntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const signed __int8& rValue) { return self.IntMultOp(rValue); }
        friend MediumDecBase operator*(MediumDecBase self, const unsigned __int64& rValue) { return self.UIntMultOp(rValue); }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator+=(const MediumDecBase& rValue) { return AddOp(rValue); }

        /// <summary>
        /// /= Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator+=(const signed int& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const unsigned int& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const unsigned __int64& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const signed __int64& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const signed __int16& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const signed __int8& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const unsigned __int16& rValue) { return IntAddOp(rValue); }
        MediumDecBase& operator+=(const unsigned __int8& rValue) { return IntAddOp(rValue); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(MediumDecBase self, const MediumDecBase& rValue) { return self.AddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const int& rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const unsigned int rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const signed long long rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const unsigned __int64 rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const __int16& rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const unsigned __int16& rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const signed __int8& rValue) { return self.IntAddOp(rValue); }
        friend MediumDecBase operator+(MediumDecBase self, const unsigned __int64& rValue) { return self.IntAddOp(rValue); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator-=(const MediumDecBase& rValue) { return SubOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between MediumDecBase and Integer Value
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase& operator-=(const signed int& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const unsigned int& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const unsigned __int64& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const signed __int64& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const signed __int16& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const signed __int8& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const unsigned __int16& rValue) { return IntSubOp(rValue); }
        MediumDecBase& operator-=(const unsigned __int8& rValue) { return IntSubOp(rValue); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase self, const MediumDecBase& rValue) { return self.SubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const int& rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const unsigned int rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const signed long long rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const unsigned __int64 rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const __int16& rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const unsigned __int16& rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const signed __int8& rValue) { return self.IntSubOp(rValue); }
        friend MediumDecBase operator-(MediumDecBase self, const unsigned __int64& rValue) { return self.IntSubOp(rValue); }

    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

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
            IntHalfAdditionOp(One);
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
            IntHalfSubtractionOp(One);
            return *this;
        }

        /// <summary>
        /// MediumDecBase++ Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator ++(int)
        {
            MediumDecBase tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecBase-- Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator --(int)
        {
            MediumDecBase tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecBase* Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator *()
        {
            return *this;
        }

        /// <summary>
        /// LeftShift operation lValue*(2ToPowOf(rValue))
        /// (Variation without modifying owner object)
        /// Based on (https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase operator<<(const MediumDecBase& rValue)//MediumDecBase& operator<<(const MediumDecBase& rValue)
        {//Allowing negative shift operations based on formula instead of returning undefined
            MediumDecBase rightSideMultiplier = Two;
            rightSideMultiplier.IntPowOp(rValue);
            return MultipleByUnsigned(rightSideMultiplier);
        }

        /// <summary>
        /// RightShift operation lValue/(2ToPowOf(rValue))
        /// (Variation without modifying owner object)
        /// Based on (https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase operator>>(const MediumDecBase& rValue)//operator<<(const MediumDecBase& rValue)
        {//Allowing negative shift operations based on formula instead of returning undefined
            MediumDecBase rightSideDivisor = Two;
            rightSideDivisor.IntPowOp(rValue);
            return DivideByUnsigned(rightSideDivisor);
        }

    #pragma endregion Other Operators

    #pragma region Modulus Operations
    #if !defined(AltNum_PreventModulusOverride)
        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        template<IntegerType IntType=signed int>
        MediumDecBase BasicIntRemOp(IntType rValue)
        {
            if (DecimalHalf == 0)
                IntValue %= rValue;
            else
            {
                if (rValue < 0)
                    rValue *= -1;
                __int64 SRep = DecimalOverflowX * IntHalfAsAbs() + DecimalHalf;
                __int64 divRes = SRep / rValue;
                __int64 C = SRep - rValue * divRes;
                if (C == 0)
                    SetAsZero();
                else
                {
                    divRes = C / DecimalOverflowX;
                    C = C - DecimalOverflowX * divRes;
                    IntValue = divRes;
                    DecimalHalf = C;
                }
            }
            return *this;
        }

        MediumDecBase Int32BasicRemOp(signed int& RValue) { return BasicIntRemOp(RValue); }
        MediumDecBase UInt32BasicRemOp(unsigned int& RValue) { return BasicIntRemOp(RValue); }
        MediumDecBase Int64BasicRemOp(signed long long& RValue) { return BasicIntRemOp(RValue); }
        MediumDecBase UInt64BasicRemOp(unsigned long long& RValue) { return BasicIntRemOp(RValue); }

        MediumDecBase Int32BasicRem(signed int RValue) { MediumDecBase self = *this; return BasicIntRemOp(RValue); }
        MediumDecBase UInt32BasicRem(unsigned int RValue) { MediumDecBase self = *this; return BasicIntRemOp(RValue); }
        MediumDecBase Int64BasicRem(signed long long RValue) { MediumDecBase self = *this; return BasicIntRemOp(RValue); }
        MediumDecBase UInt64BasicRem(unsigned long long RValue) { MediumDecBase self = *this; return BasicIntRemOp(RValue); }

        static MediumDecBase Int32BasicModulus(MediumDecBase self, signed int RValue) { return self.BasicIntRemOp(RValue); }
        static MediumDecBase UInt32BasicModulus(MediumDecBase self, unsigned int RValue) { return self.BasicIntRemOp(RValue); }
        static MediumDecBase Int64BasicModulus(MediumDecBase self, signed long long RValue) { return self.BasicIntRemOp(RValue); }
        static MediumDecBase UInt64BasicModulus(MediumDecBase self, unsigned long long RValue) { return self.BasicIntRemOp(RValue); }

        //Performs modulus operation based on "C = A - B * (A / B)" formula
        template<IntegerType IntType=signed int>
        MediumDecBase IntRemOp(const IntType& rValue)
        {
            MediumDecBase divRes = DivideByInt(rValue);
            MediumDecBase C = *this - divRes.MultipleByInt(rValue);
            return C;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        MediumDecBase Int32RemOp(signed int& RValue) { return IntRemOp(RValue); }
        MediumDecBase UInt32RemOp(unsigned int& RValue) { return IntRemOp(RValue); }
        MediumDecBase Int64RemOp(signed long long& RValue) { return IntRemOp(RValue); }
        MediumDecBase UInt64RemOp(unsigned long long& RValue) { return IntRemOp(RValue); }

        MediumDecBase Int32Rem(signed int RValue) { MediumDecBase self = *this; return self.IntRemOp(RValue); }
        MediumDecBase UInt32Rem(unsigned int RValue) { MediumDecBase self = *this; return self.IntRemOp(RValue); }
        MediumDecBase Int64Rem(signed long long RValue) { MediumDecBase self = *this; return self.IntRemOp(RValue); }
        MediumDecBase UInt64Rem(unsigned long long RValue) { MediumDecBase self = *this; return self.IntRemOp(RValue); }

        static MediumDecBase Int32Modulus(MediumDecBase self, signed int RValue) { return self.IntRemOp(RValue); }
        static MediumDecBase UInt32Modulus(MediumDecBase self, unsigned int RValue) { return self.IntRemOp(RValue); }
        static MediumDecBase Int64Modulus(MediumDecBase self, signed long long RValue) { return self.IntRemOp(RValue); }
        static MediumDecBase UInt64Modulus(MediumDecBase self, unsigned long long RValue) { return self.IntRemOp(RValue); }


        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        MediumDecBase& BasicRemOp(const MediumDecBase& rValue)
        {
            if (rValue.DecimalHalf == 0)
            {
                if (DecimalHalf == 0)
                {
                    IntValue %= rValue.IntValue;
                }
                else
                {
                    __int64 SRep = DecimalOverflowX * IntHalfAsAbs() + DecimalHalf;
                    __int64 divRes; __int64 C;
                    if (rValue < 0)
                    {
                        divRes = SRep / -rValue.IntValue;
                        C = SRep - (-rValue.IntValue * divRes);
                    }
                    else
                    {
                        divRes = SRep / rValue.IntValue;
                        C = SRep - (rValue.IntValue * divRes);
                    }
                    if (C == 0)
                        SetAsZero();
                    else
                    {
                        divRes = C / DecimalOverflowX;
                        C = C - DecimalOverflowX * divRes;
                        IntValue = divRes;
                        DecimalHalf = C;
                    }
                }
            }
            else
            {
                MediumDecBase divRes = *this / rValue;
                MediumDecBase C = *this - rValue * divRes;
                IntValue = C.IntValue; DecimalHalf = C.DecimalHalf;
                ExtraRep = C.ExtraRep;
            }
        }

        MediumDecBase BasicRem(MediumDecBase rValue) { MediumDecBase self = *this; BasicRemOp(rValue); return self; }

        /// <summary>
        //	Performs modulus operation based on "C = A - B * (A / B)" formula
        /// </summary>
        /// <param name="rValue">The rightside value.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase RemOp(const MediumDecBase& rValue)
        {
            MediumDecBase divRes = *this / rValue;
            MediumDecBase C = *this - rValue * divRes;
            return C;
        }

        MediumDecBase Rem(const MediumDecBase& rValue) { MediumDecBase self = *this; return self.RemOp(rValue); }

        /// <summary>
        /// Modulus Operation (Division operation that returns the remainder result)
        /// </summary>
        /// <param name="self">The leftside value.</param>
        /// <param name="Value">The rightside value.</param>
        /// <returns>MediumDecBase&</returns>
        static MediumDecBase Modulus(MediumDecBase self, MediumDecBase Value) { return self.RemOp(Value); }

		void CatchAllRem(MediumDecBase& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicRemOp(convertedRVal);
		}
		
		void CatchAllRemV2(MediumDecBase& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			MediumDecBase convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicRemOp(convertedRVal);
		}
		
		void CatchAllRemV3(MediumDecBase& rValue)
		{
			ConvertToNormTypeV2();
            MediumDecBase convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicRemOp(convertedRVal);
		}

        friend MediumDecBase operator%=(MediumDecBase& self, MediumDecBase Value) { return self.RemOp(Value); }
        friend MediumDecBase operator%=(MediumDecBase& self, int Value) { return self.Int32RemOp(Value); }
        friend MediumDecBase operator%=(MediumDecBase& self, unsigned int Value) { return self.UInt32RemOp(Value); }
        friend MediumDecBase operator%=(MediumDecBase& self, signed long long Value) { return self.Int64RemOp(Value); }
        friend MediumDecBase operator%=(MediumDecBase& self, unsigned __int64 Value) { return self.UInt64RemOp(Value); }

        friend MediumDecBase operator%(MediumDecBase self, MediumDecBase Value) { return self.RemOp(Value); }
        friend MediumDecBase operator%(MediumDecBase self, int Value) { return self.Int32RemOp(Value); }
        friend MediumDecBase operator%(MediumDecBase self, unsigned int Value) { return self.UInt32RemOp(Value); }
        friend MediumDecBase operator%(MediumDecBase self, signed long long Value) { return self.Int64RemOp(Value); }
        friend MediumDecBase operator%(MediumDecBase self, unsigned __int64 Value) { return self.UInt64RemOp(Value); }


        #if defined(AltNum_EnableAlternativeModulusResult)//Return AltNumModChecker<MediumDecBase> Result with both Remainder and division result
        template<IntegerType IntType=signed int>
        static AltNumModChecker<MediumDecBase> IntModulusOp(IntType RValue)
        {
            AltNumModChecker<MediumDecBase> Res;
            Res.CalcIfZero(*this, AltNum(RValue));
            return Res;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        AltNumModChecker<MediumDecBase> ModulusOp(signed int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<MediumDecBase> ModulusOp(unsigned int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<MediumDecBase> ModulusOp(signed long long& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<MediumDecBase> ModulusOp(unsigned long long& RValue) { IntModulusOp(RValue); }

        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBase& self, signed int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBase& self, unsigned int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBase& self, signed long long& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBase& self, unsigned long long& RValue) { self.IntModulusOp(RValue); }


        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBaseVariant RValue)
        {
            AltNumModChecker<MediumDecBase> Res;
            Res.CalcIfZero(*this, RValue);
            return Res;
        }

        static AltNumModChecker<MediumDecBase> ModulusOp(MediumDecBase& self, MediumDecBase& RValue) { self.ModulusOp(RValue); }
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
    #pragma endregion Bitwise Functions

	#pragma region Math Etc Functions
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase& Abs()
        {
            if (IntValue == NegativeRep)
                IntValue = 0;
            else if (IntValue < 0)
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
        MediumDecBase& FloorOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                ConvertPiToNum();
                break;
            case RepType::ApproachingBottom:
            case RepType::ApproachingTop:
                if (IntValue == NegativeRep)
                    IntValue = 0;
                ExtraRep = 0;
                return *this;
                break;
            default:
                ConvertToNormTypeV2();
                break;
            }
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static MediumDecBase Floor(MediumDecBase Value, int precision=0)
        {
            Value.ConvertToNormTypeV2();
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
        MediumDecBase& CeilOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                ConvertPiToNum();
                break;
            case RepType::ApproachingBottom:
            case RepType::ApproachingTop:
                if (IntValue == NegativeRep)
                    IntValue = 0;
                else
                    ++IntValue;
                ExtraRep = 0;
                return *this;
                break;
            default:
                //ConvertToNormTypeV2();//Prevent losing imaginery numbers
                break;
            }
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

        static MediumDecBase Ceil(MediumDecBase Value) { return Value.CeilOp(); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static int FloorInt(MediumDecBaseVariant Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                Value.ConvertPiToNum();
                break;
            //case RepType::ApproachingTop:
            //case RepType::ApproachingBottom:
            //    return Value.IntValue;
            //    break;
            default:
                Value.ConvertToNormTypeV2();
                break;
            }
            if (Value.DecimalHalf == 0)
            {
                return Value.GetIntHalf();
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.GetIntHalf() - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        int CeilIntOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
				case RepType::NormalType:
					break;
				case RepType::PiNum:
				{
					ConvertPiToNum();//return CeilInt(ConvertPiToNum());
					break;
				}
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*E Representation
			#else
				case RepType::EFractional://  IntValue/DecimalHalf*E Representation
			#endif
		#endif
					ConvertToNormType(repType);
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
					ConvertToNormalIRepV2();
					break;
		#endif
	#endif
				//case RepType::ApproachingTop:
				//case RepType::ApproachingBottom:
				default:
					ConvertToNormType(repType);
					break;
            }
            if (DecimalHalf == 0)
            {
                return GetIntHalf();
            }
            if (IntValue == NegativeRep) { return 0; }
            else
            {
                return GetIntHalf() + 1;
            }
        }
        
        static int CeilInt(MediumDecBase Value) { return Value.CeilIntOp(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& Trunc()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            else if (DecimalHalf == ApproachingBottomRep)
            {
                DecimalHalf = 0; ExtraRep = 0;
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                DecimalHalf = 999999999; ExtraRep = 0;
            }
            else
            {
#endif
                ConvertToNormTypeV2();
                DecimalHalf = 0;
#if defined(AltNum_EnableInfinityRep)
            }
#endif
            return *this;
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Trunc(MediumDecBaseVariant Value)
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
                switch (value.GetIntHalf())
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
		    value.ConvertToNormTypeV2();
			BasicSqrt(value, precision);
		}

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=signed int>
        MediumDecBase BasicIntPowOp(const IntType& expValue)
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

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=signed int>
        MediumDecBase BasicUIntPowOp(const IntType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
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

        template<IntegerType IntType=signed int>
        MediumDecBase BasicIntPow(const IntType& expValue) { MediumDecBase self = *this; return self.BasicIntPowOp(expValue); }

        template<IntegerType IntType=signed int>
        MediumDecBase BasicUIntPow(const IntType& expValue) { MediumDecBase self = *this; return self.BasicUIntPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=signed int>
        MediumDecBase IntPowOp(const IntType& expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    tSetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    return Zero;
                else if (IntValue == -1 && expValue % 2 == 0)
                    IntValue = 1;
                else
                    return *this;//Returns infinity
                return *this;
            }
            else
                return BasicIntPowOp(expValue);
        }

        template<IntegerType IntType=signed int>
        MediumDecBase UIntPowOp(const IntType& expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    tSetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (IntValue == -1 && expValue % 2 == 0)
                    IntValue = 1;
                else
                    return *this;//Returns infinity
                return *this;
            }
            else
                return BasicUIntPowOp(expValue);
        }

        template<IntegerType IntType=signed int>
        MediumDecBase IntPow(const IntType& expValue) { MediumDecBase self = *this; return self.IntPowOp(expValue); }

        template<IntegerType IntType=signed int>
        MediumDecBase UnsignedIntPow(const IntType& expValue) { MediumDecBase self = *this; return self.UIntPowOp(expValue); }
public:
        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase NthRootOp(int& n, MediumDecBase& precision = MediumDecBase::JustAboveZero)
        {
            //Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            MediumDecBase xPre = One;
            xPre += (*this - One)/n;
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            MediumDecBase delX = MediumDecBase(2147483647, 0);

            //  xK denotes current value of x 
            MediumDecBase xK;
            MediumDecBase xPrePower;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = xPre * nMinus1;
                xPrePower = xPre.IntPow(nMinus1);
                xK += DivideBy(xPrePower); //*this/xPrePower;
                xK.IntDivOp(n);
                delX = MediumDecBase::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        MediumDecBase NthRoot(MediumDecBase value, int n, MediumDecBase precision = MediumDecBase::JustAboveZero) { return value.NthRootOp(n, precision); }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        MediumDecBase FractionalPow(int expNum, int expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        MediumDecBase FractionalPow(boost::rational<int>& Frac);

        MediumDecBase BasicPowOp(MediumDecBase& expValue);

        MediumDecBase BasicPow(MediumDecBase expValue) { MediumDecBase self = *this; return self.BasicPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase PowOp(MediumDecBase& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase Pow(MediumDecBase expValue) { MediumDecBase self = *this; return self.PowOp(expValue); }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecBase PowV2(MediumDecBase targetValue, MediumDecBase expValue) { return targetValue.PowOp(expValue); }
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
            MediumDecBase x[2] = { (MediumDecBase::One / n) * ((targetValue*nMinus1) + (targetValue / targetValue.IntPow(nMinus1))), targetValue };
            while (MediumDecBase::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (MediumDecBase::One / n) * ((x[1]*nMinus1) + (targetValue / x[1].IntPow(nMinus1)));
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
            //x.ConvertToNormTypeV2();//Prevent losing imaginary number status
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
            RepType repType = value.GetRepType();
            MediumDecBase ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
        }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
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
            RepType repType = value.GetRepType();
            MediumDecBase ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
            if(ConvertedVal.IntValue==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
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
            else if (ConvertedVal.IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
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
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Ln(MediumDecBaseVariant value)
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
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Log10(MediumDecBaseVariant value)
        {
            if (value == MediumDecBase::One)
                return MediumDecBase::Zero;
            RepType repType = value.GetRepType();
            MediumDecBase ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecBase(index, 0);
                }
                return MediumDecBase(9, 0);
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
        static MediumDecBase IntegerLog10(ValueType value)
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
            RepType repType = value.GetRepType();
            MediumDecBase ConvertedVal;
            switch (repType)
            {
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
		#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
	#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
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
        static MediumDecBase IntegerLog(MediumDecBase value, int baseVal)
        {
            RepType repType = value.GetRepType();
            MediumDecBase ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
        }
            if (ConvertedVal == MediumDecBase::One)
                return MediumDecBase::Zero;
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
                    AddRes = W.IntPow(WPow) / WPow;
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
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase SinFromAngle(MediumDecBaseVariant Value)
        {
    #if defined(AltNum_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
        #if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
        #elif defined(AltNum_EnableUndefinedButInRange)
            //Return undefined value between -1 and 1
        #else
                throw "Operation results in NaN";
        #endif
    #endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
		#endif
                Value.ConvertToNormalIRep(repType);
                break;
	#endif
            default:
                Value.ConvertToNormType(repType);
                break;
            }
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
                    NewValue += Radius.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                }//x[1].IntPow(nMinus1)
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase CosFromAngle(MediumDecBaseVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
#if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltNum_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
#endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
		#endif
                Value.ConvertToNormalIRep(repType);
                break;
	#endif
            default:
                Value.ConvertToNormType(repType);
                break;
            }
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
                    NewValue += Radius.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
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
        static MediumDecBase BasicSinOperation(MediumDecBase& Value)
        {
            MediumDecBase SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += Value.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Sin(MediumDecBaseVariant Value)
        {
/*
            if (Value.ExtraRep == PiRep)
            {


            }
#if defined(AltNum_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)

#endif*/
            RepType repType = Value.GetRepType();
            switch (repType)
            {
                case RepType::PiNum://0 to 2Pi range (2Pi == 0Pi)
                {
                    if (Value.IntValue < 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                        }
                        else
                        {
                            Value.IntValue *= -1;
                            Value.IntValue %= 2;
                            Value.IntValue = 2 - Value.IntValue;
                            if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                        }
                    }
                    else
                    {
                        Value.IntValue %= 2;
                    }
                    if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and Pi
                        return Zero;
                    if (Value.DecimalHalf == 500000000)//0.5 Pi = 1; 1.5Pi = -1
                        return Value.IntValue==0?NegativeOne:One;
                    MediumDecBase SinValue = Zero;
                    for (int i = 0; i < 7; ++i)
                    {
                        SinValue += Value.Pow(2 * i + 1) * (i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                    }
                    return SinValue;
                    break;
                }
		#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnablePiPowers)
			#if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            #elif defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
			#else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			#endif
		    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            #endif
                {
                    Value.ConvertToPiRep(repType);
                    if (Value.IntValue < 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                        }
                        else
                        {
                            Value.IntValue *= -1;
                            Value.IntValue %= 2;
                            Value.IntValue = 2 - Value.IntValue;
                            if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                        }
                    }
                    else
                    {
                        Value.IntValue %= 2;
                    }
                    if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and Pi
                        return Zero;
                    if (Value.DecimalHalf == 500000000)//0.5 Pi = 1; 1.5Pi = -1
                        return Value.IntValue==0?NegativeOne:One;
                    MediumDecBase SinValue = Zero;
                    for (int i = 0; i < 7; ++i)
                    {
                        SinValue += Value.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                    }
                    return SinValue;
                    break;
                }
		#endif
#if defined(AltNum_EnableInfinityRep)
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
#if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltNum_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
                    break;
#endif
            default:
                break;
            }
            return BasicSinOperation(Value);
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase BasicCosOperation(MediumDecBase& Value)
        {
            MediumDecBase CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Cos(MediumDecBaseVariant Value)
        {
/*
            if (Value.ExtraRep == PiRep)//0 to 2Pi range (2Pi == 0Pi)
            {
                if (Value.IntValue < 0)
                {
                    if (Value.IntValue == NegativeRep)
                    {
                        Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                    }
                    else
                    {
                        Value.IntValue *= -1;
                        Value.IntValue %= 2;
                        Value.IntValue = 2 - Value.IntValue;
                        if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                    }
                }
                else
                {
                    Value.IntValue %= 2;
                }
                //Cos table listed on https://byjus.com/maths/value-of-cos-180/
                if (Value.DecimalHalf == 0)//cos(0) = 1;cos(Pi) = -1;
                    return Value.IntValue == 0?One:NegativeOne;
                if (Value.DecimalHalf == 500000000)//cos(0.5) Pi = 0; cos(1.5Pi) = 0;
                    return Zero;
            }
    #if defined(AltNum_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)
        #if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
        #elif defined(AltNum_EnableUndefinedButInRange)
            //Return undefined value between -1 and 1
        #else
                throw "Operation results in NaN";
        #endif
    #endif
*/
            return BasicCosOperation(Value);
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase Tan(MediumDecBaseVariant Value)
        {
            MediumDecBase SinValue = Zero;
            MediumDecBase CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase TanFromAngle(MediumDecBaseVariant Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
#if defined(AltNum_EnableNaN)
            case RepType::NaN:
                return NaNValue();
#endif
#if defined(AltNum_EnableImaginaryNum) && defined(AltNum_EnableFractionals)
            {
                int Divisor = Value.ExtraRep*-1;
                Value.ExtraRep = 0;
                Value /= Divisor;
                Value.ExtraRep = -2147483647;
                break;
            }
#endif
#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
#endif
#if defined(AltNum_EnableERep)
            case RepType::ENum:
#if defined(AltNum_EnableFractionals)
            case RepType::ENumByDiv:
#endif
#endif
                Value.ConvertToNormTypeV2();
                break;
            //case RepType::INum:
            default:
                Value.ConvertToNormTypeV2();//Don't convert things like imaginary numbers into real numbers
                break;
            }
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
#if defined(AltNum_EnableInfinityRep)
                return Infinity;
#else
                return MediumDecBase::Maximum;//Positive Infinity
#endif
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecBase::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return MediumDecBase::NegativeInfinity;
#else
                return MediumDecBase::Minimum;//Negative Infinity
#endif
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
        template<typename MediumDecBaseVariant = MediumDecBase>
        static MediumDecBase ATan(MediumDecBaseVariant Value)
        {
            MediumDecBase SinValue = Zero;
            MediumDecBase CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
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
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            MediumDecBase coeff_1 = MediumDecBase(1, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            MediumDecBase coeff_1 = MediumDecBase(1, 4, PiByDivisorRep);
    #else
            MediumDecBase coeff_1 = MediumDecBase(0, 250000000, PiRep);//Pi / 4;
    #endif
#else
            MediumDecBase coeff_1 = PiNum / 4;
#endif
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            MediumDecBase coeff_2 = MediumDecBase(3, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            MediumDecBase coeff_2 = MediumDecBase(3, 4, PiByDivisorRep);
    #else
            MediumDecBase coeff_2 = MediumDecBase(0, 750000000, PiRep);//Pi / 4;
    #endif
#else
            MediumDecBase coeff_2 = PiNum * MediumDecBase(0, 750000000);
#endif
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

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The target value to convert into MediumDecBase</param>
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
                IntHalfAdditionOp(TempInt02);
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
        std::string Value = IntHalfAsString();
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
            return "âˆž";
            break;
        case RepType::NegativeInfinity:
            return "-âˆž";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp();
			#else
            return IntHalfAsString() + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp();
			#else
            return IntHalfAsString() + ".9..9";
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
            return BasicToStringOp()+"Ï€";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"Ï€";
            break;
        #elif defined(AltNum_EnablePiFractional)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return IntHalfAsString() +"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"Ï€";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledEFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableEFractional)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return IntHalfAsString() +"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableIFractional)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return IntHalfAsString() +"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"Ï€";
			#else
            return IntHalfAsString() + ".9..9Ï€";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"e";
			#else
            return IntHalfAsString() + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "âˆži";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-âˆži";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp()+"i";
			#else
            return IntHalfAsString() + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"i";
			#else
            return IntHalfAsString() + ".9..9i";
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
            return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"Ï€";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
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
    #if defined(AltNum_EnableNil)
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
        std::string Value = IntHalfAsString();
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
            return "âˆž";
            break;
        case RepType::NegativeInfinity:
            return "-âˆž";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp();
			#else
            return IntHalfAsString() + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp();
			#else
            return IntHalfAsString() + ".9..9";
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
            return BasicToFullStringOp()+"Ï€";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"Ï€";
            break;
        #elif defined(AltNum_EnablePiFractional)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return IntHalfAsString()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"Ï€";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToFullStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledEFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableEFractional)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return IntHalfAsString()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToFullStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableIFractional)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return IntHalfAsString() +"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"Ï€";
			#else
            return IntHalfAsString() + ".9..9Ï€";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"e";
			#else
            return IntHalfAsString() + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "âˆž";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-âˆž";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp()+"i";
			#else
            return IntHalfAsString() + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"i";
			#else
            return IntHalfAsString() + ".9..9i";
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
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"Ï€";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
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
    #if defined(AltNum_EnableNil)
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

}