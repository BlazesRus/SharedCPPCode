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

#ifdef AltNum_EnableMediumDecBasedSetValues
    #include "..\MediumDec\MediumDec.hpp"
#endif

#include "..\MediumDecV2\MediumDecV2Base.hpp"
#include "AltDecPreprocessors.h"
#include "MirroredIntV2.hpp"

namespace BlazesRusCode
{
    class AltDecBase;

/*---Accuracy Tests(with AltDecBase based settings):
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
    class DLL_API AltDecBase: public virtual MediumDecBaseV2
    {
    public:
        /// <summary>
        /// (Used exclusively for alternative represents of numbers including imaginary numbers and for fractionals)
        /// If ExtraRep>0 and DecimalHalf.Value<AlternativeFractionalLowerBound(and ExtraRep.Value<=FractionalMaximum), then ExtraRep acts as denominator
        /// If ExtraRep is Negative and DecimalHalf.Value<AlternativeFractionalLowerBound, then AltDecBase represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf.Value<999999999, then AltDecBase represents +- 2147483647.999999999
        /// </summary>
        MirroredIntV2 ExtraRep;

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltDecBase(const IntHalfType& intVal, const DecimalHalfType& decVal = 0, const MirroredIntV2& extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        AltDecBase(const AltDecBase&) = default;

        AltDecBase& operator=(const int& rhs)
        {
            IntValue = rhs; DecimalHalf = 0;
            ExtraRep = 0;
            return *this;
        } const

        AltDecBase& operator=(const AltDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = rhs.ExtraRep;
            return *this;
        } const

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const AltDecBase& Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        } const

        virtual void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0; ExtraRep = 0;
        }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;

    protected:
        #pragma region Const Representation values
	#if defined(AltNum_EnableApproachingDivided)
        //When DecimalHalf.Value equals this value, it represents Approaching IntValue from right towards left (IntValue.0..01)/ExtraRep.Value
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int ApproachingBottomDivRep = 2147483644;
		#else
        static const unsigned int ApproachingBottomDivRep = 1073741817;
        #endif
		//When DecimalHalf.Value equals this value, it represents Approaching IntValue from left towards right (IntValue.9..9)/ExtraRep.Value
		#if defined(AltNum_UseIntForDecimalHalf)
		static const signed int ApproachingTopDivRep = 2147483643;
		#else
		static const unsigned int ApproachingTopDivRep = 1073741816;
		#endif
	#endif
    #if defined(AltNum_EnableWithinMinMaxRange)
        //Undefined but in ranged of IntValue to DecimalHalf when at this ExtraRep.Value
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int WithinMinMaxRangeRep = -2147483648;
		#else
        static const unsigned int WithinMinMaxRangeRep = 2147483648;
		#endif
    #endif
    #if defined(AltNum_UseIntForDecimalHalf)
        static const signed int AlternativeFractionalLowerBound = -2147483640;
    #else
        static const unsigned int AlternativeFractionalLowerBound = 1073741816;
    #endif
        //Upper limit for Mixed Fractions and Fractionals; infinite approaching type representations at and after this DecimalHalf value
   #if defined(AltNum_UseIntForDecimalHalf)
        static const signed int InfinityBasedLowerBound = 2147483643;
   #else
        static const unsigned int InfinityBasedLowerBound = 1073741816;
   #endif
	//Fractional Division Maximum at this ExtraRep.Value
    #if defined(AltNum_EnableWithinMinMaxRange)
        static const signed int FractionalMaximum = 2147483647;
	#else
        static const unsigned int FractionalMaximum = 2147483648;
	#endif
	
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType GetRepType()
        {
#if !defined(AltNum_UseIntForDecimalHalf)
            if(DecimalHalf.Flag==0)
			{
                return RepType::NormalType;
			}
    #if defined(AltNum_EnablePiRep)
            else if(DecimalHalf.Flag==1)
            {
				return RepType::PiNum;
            }
    #endif
    #if defined(AltNum_EnableERep)
            else if(DecimalHalf.Flag==2)
            {
				return RepType::ENum;
            }
    #endif
	#if defined(AltNum_EnableImaginaryNum)
            else if(DecimalHalf.Flag==3)
            {
				return RepType::INum;
            }
	#elif defined(AltNum_EnableWithinMinMaxRange)
            else if(DecimalHalf.Flag==3)
				//If IntValue==NegativeRep, then left side range value equals negative infinity
				//If DecimalHalf.Value==InfinityRep, then right side range value equals positive infinity
				return IntHalf==0&&DecimalHalf.Value==0? RepType::UndefinedButInRange: RepType::WithinMinMaxRange;
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
				//If DecimalHalf equals InfinityRep, than equals undefined value with range between negative infinity and positive infinity (negative range values indicates inverted range--any but the range of values)
                return RepType::UndefinedButInRange;
	#endif
	#if defined(AltNum_EnableNaN)
			else if(DecimalHalf==NaNRep)
				return RepType::NaN;
			else if(DecimalHalf==UndefinedRep)
				return RepType::Undefined;
	#endif
	#if defined(AltNum_EnableNil)
			else if(DecimalHalf==NilRep)
				return RepType::Nil;
    #endif
            else
				throw "Unknown or non-enabled representation type detected";
#else
        #if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
            {
            #if defined(AltNum_EnableImaginaryInfinity)
                if (ExtraRep == IRep)
                    return IntValue==1?RepType::PositiveImaginaryInfinity:RepType::NegativeImaginaryInfinity;
                else
            #endif
            #if defined(AltNum_EnableUndefinedButInRange)
                if (ExtraRep == UndefinedInRangeRep)
                    return RepType::UndefinedButInRange;
                else
            #endif
            #if defined(WithinMinMaxRangeRep)
                if (ExtraRep == WithinMinMaxRangeRep)
                    return RepType::WithinMinMaxRange;
                else
            #endif
                    return IntValue==1?RepType::PositiveInfinity:RepType::NegativeInfinity;
            }
            else
        #endif
        #if defined(AltNum_EnableApproachingValues)//old value = ApproachingValRep
            if (DecimalHalf == ApproachingBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
            #if defined(AltNum_EnableApproachingDivided)//if(ExtraRep>1)
                    return RepType::ApproachingMidLeft;//ExtraRep value of 2 results in 0.49999...9
            #else
                    throw "EnableApproachingDivided feature not enabled";
            #endif
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
            #if defined(AltNum_EnableApproachingPi)
                else if (ExtraRep == PiRep)
                    return RepType::ApproachingTopPi;
            #endif
            #if defined(AltNum_EnableApproachingE)
                else if (ExtraRep == ERep)
                    return RepType::ApproachingTopE;
            #endif
                else
            #if defined(AltNum_EnableApproachingDivided)
                    return RepType::ApproachingMidRight;//ExtraRep value of 2 results in 0.500...1
            #else
                    throw "EnableApproachingDivided feature not enabled";
            #endif
            }
            #if defined(AltNum_EnableImaginaryInfinity)//ApproachingImaginaryValRep
            else if (DecimalHalf == ApproachingImaginaryBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
                #if defined(AltNum_EnableApproachingDivided)
                    return RepType::ApproachingImaginaryMidLeft;//ExtraRep value of 2 results in 0.49999...9
                #else
                    throw "EnableApproachingDivided feature not enabled";
                #endif
            }
            else if (DecimalHalf == ApproachingImaginaryTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
                else
                #if defined(AltNum_EnableApproachingDivided)
                    return RepType::ApproachingImaginaryMidRight;//ExtraRep value of 2 results in 0.500...1
                #else
                    throw "EnableApproachingDivided feature not enabled";
                #endif
            }
            #endif
        #endif
            if(ExtraRep==0)
            {
    #if defined(AltNum_EnableNaN)
                if(DecimalHalf==NaNRep)
                    return RepType::NaN;
                else if(DecimalHalf==UndefinedRep)
                    return RepType::Undefined;
    #endif
                return RepType::NormalType;
            }
            else if(IntValue==0&&DecimalHalf==0)
            {
                return RepType::NormalType;
            }
    #ifdef AltNum_EnablePiRep
            else if(ExtraRep==PiRep)
                return RepType::PiNum;
        #if defined(AltNum_EnablePiFractional)
            else if(ExtraRep==PiByDivisorRep)
                return RepType::PiFractional;
        #endif
    #endif
            else if(ExtraRep>0)
            {
    #if defined(AltNum_EnableMixedFractional)
                if(DecimalHalf<0)
                    return RepType::MixedFrac;
    #endif
    #if defined(AltNum_EnableFractionals)
                return RepType::NumByDiv;
    #endif
                throw "Non-enabled representation detected";
            }
    #if defined(AltNum_EnableERep)
            else if(ExtraRep==ERep)
            {
                return RepType::ENum;
            }
        #if defined(AltNum_EnableEFractional)
            else if(ExtraRep==EByDivisorRep)//(IntValue/DecimalHalf)*e
                return RepType::EFractional;
        #endif
    #endif

    #if defined(AltNum_EnableImaginaryNum)
            else if(ExtraRep==IRep)
            {
                return RepType::INum;
            }
        #if defined(AltNum_EnableIFractional)
            else if(ExtraRep==IByDivisorRep)
                    return RepType::IFractional;
        #endif
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
           else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;//If DecimalHalf equals InfinityRep, than equals undefined value with range between negative infinity and positive infinity (negative range values indicates inverted range--any but the range of values)
        #if defined(AltNum_EnableWithinMinMaxRange)
            //If IntValue==NegativeRep, then left side range value equals negative infinity
            //If DecimalHalf==InfinityRep, then right side range value equals positive infinity
           else if(ExtraRep==WithinMinMaxRangeRep)
                return RepType::WithinMinMaxRange;
        #endif
    #endif
            else if(ExtraRep<0)
    #if defined(AltNum_EnableAlternativeMixedFrac)
                if(DecimalHalf<0)
        #if defined(AltNum_EnableMixedPiFractional)
                    return RepType::MixedPi;
        #elif defined(AltNum_EnableMixedEFractional)
                    return RepType::MixedE;
        #elif defined(AltNum_EnableMixedIFractional)
                    return RepType::MixedI;
        #else
                    throw "Non-enabled Alternative Mixed Fraction representation type detected";
        #endif
                else
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
                    return RepType::PiNumByDiv;
    #elif defined(AltNum_EnableDecimaledEFractionals)
                    return RepType::ENumByDiv;
    #elif defined(AltNum_EnableDecimaledIFractionals)
                    return RepType::INumByDiv;
    #else
                    throw "Non-enabled Negative ExtraRep representation type detected";
    #endif
            else
                throw "Unknown or non-enabled representation type detected";
#endif
            return RepType::UnknownType;//Catch-All Value;
        }

    #pragma endregion RepType

public:
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
        template<MediumDecVariant VariantType=AltDecBase>
        virtual void SetPiVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            ExtraRep = 0;
        }
        
        virtual void SetPiValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,1);
            ExtraRep = 0;
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(AltNum_EnableERep)
        template<MediumDecVariant VariantType=AltDecBase>
        virtual void SetEVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            ExtraRep = 0;
        }
        
        virtual void SetEValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,2);
            ExtraRep = 0;
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(AltNum_EnableIRep)
        template<MediumDecVariant VariantType=AltDecBase>
        virtual void SetIVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
            ExtraRep = 0;
        }
        
        virtual void SetIValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,3);
            ExtraRep = 0;
        }
    #endif
    #pragma endregion INum Setters

    #pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        void SetAsInfinity()
        {
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsNegative = 0; DecimalHalf = InfinityRep;
    #else
            IntValue = 1; DecimalHalf = InfinityRep;
    #endif
            ExtraRep = 0;
        }

        void SetAsNegativeInfinity()
        {
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsNegative = 1; DecimalHalf = InfinityRep;
    #else
            IntValue = -1; DecimalHalf = InfinityRep;
    #endif
            ExtraRep = 0;
        }
	#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
	#if defined(AltNum_EnableApproachingValues)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottom(int value=0)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
            ExtraRep = 0;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTop(int value=0)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = 0;
        }
        #endif
		
		#if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPi(int value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
            ExtraRep = 0;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopE(int value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
            ExtraRep = 0;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopI(int value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
            ExtraRep = 0;
        }
		#endif
    #endif
    //Separating into separate methods since second parameter not same as previous virtual function
    #if defined(AltNum_EnableApproachingDivided)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottomDiv(int value, int divisor)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
             ExtraRep = divisor;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTopDiv(int value, int divisor)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
             ExtraRep = divisor;
        }
        #endif
		
		#if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPiDiv(int value, int divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
             ExtraRep = divisor;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopEDiv(int value, int divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
             ExtraRep = divisor;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopIDiv(int value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
             ExtraRep = 0;
        }
		#endif
    #endif
    #pragma endregion ApproachingZero Setters

    #pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep; ExtraRep = 0;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep; ExtraRep = 0;
        }
	#endif
    #pragma endregion NaN Setters

    #pragma region ValueDefines

    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

    #pragma region NormalRep AltNum Division Operations

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region NormalRep AltNum Multiplication Operations

    #pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other Division Operations

	#pragma endregion Other Division Operations	

    #pragma region Other Multiplication Operations

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

    #pragma endregion Other Subtraction Operations
    }
    #pragma region String Function Source

    #pragma endregion String Function Source
}
