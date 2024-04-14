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
#include "..\..\DLLAPI.h"
#endif

#include "..\MediumDecV2\MediumDecV2Base.hpp"
#include "..\MediumDecV2\MediumDecV2.hpp"
#include "AltDecPreprocessors.h"

namespace BlazesRusCode
{
    class AltDecBase;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for fractal operations, and optionally other representations like Pi,e, and imaginary numbers.
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
        MirroredInt ExtraRep;

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltDecBase(const IntHalfType& intVal, const DecimalHalfType& decVal = 0, const MirroredInt& extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        AltDecBase(const AltDecBase&) = default;

        AltDecBase& operator=(const int& rhs)
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
            ExtraRep = 0;
            return *this;
        } const

        AltDecBase& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = 0;
            return *this;
        } const

        AltDecBase& operator=(const MediumDecV2& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
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
            switch(DecimalHalf.Flag)
            {
#endif
		#if defined(AltNum_EnablePiRep)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 1:
                    {
                #if defined(AltNum_EnableApproachingPi)
                        if (DecimalHalf == ApproachingTopRep)
                    #if defined(AltNum_EnableApproachingAlternativeDiv)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeftPi;
                            else
                    #endif
                                return RepType::ApproachingTopPi;
                #endif
                #if defined(AltNum_EnableApproachingAlternativeDiv)
                        else if (DecimalHalf == ApproachingBottomRep)//ExtraRep!=0
                    #if defined(AltNum_EnableApproachingAlternativeDiv)
                             return RepType::ApproachingMidRightPi;
                    #endif
                #endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::PiPower;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(AltNum_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedPi;
                            else
                    #endif
                                return RepType::PiNumByDiv;
                #endif
                        return RepType::PiNum;
                    }
                    break;
            #endif
        #endif
		#if defined(AltNum_EnableERep)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 2:
                    {
                #if defined(AltNum_EnableApproachingE)
                        if (DecimalHalf == ApproachingTopRep)
                    #if defined(AltNum_EnableApproachingAlternativeDiv)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeftE;
                            else
                    #endif
                                return RepType::ApproachingTopE;
                    #if defined(AltNum_EnableApproachingAlternativeDiv)
                        else if (DecimalHalf == ApproachingBottomRep)//ExtraRep!=0
                             return RepType::ApproachingMidRightE;
                    #endif
                #endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::EPower;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(AltNum_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedE;
                            else
                    #endif
                                return RepType::ENumByDiv;
                #endif
                        return RepType::ENum;
                    }
                    
                    break;
            #endif
		#endif
        #if defined(AltNum_EnableImaginaryNum)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 3:
                    {
                #if defined(AltNum_EnableImaginaryInfinity)
                        if(DecimalHalf == InfinityRep)
                            return RepType::ImaginaryInfinity;
                #endif
                #if defined(AltNum_EnableApproachingI)
                        if (DecimalHalf == ApproachingBottomRep)
                        {
                    #if defined(AltNum_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingImaginaryMidLeft;
                            else
                    #endif
                            return RepType::ApproachingImaginaryBottom;
                        }
                        else if (DecimalHalf == ApproachingTopRep)
                        {
                    #if defined(AltNum_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingImaginaryMidRight;
                            else
                    #endif
                            return RepType::ApproachingImaginaryTop;
                        }
                #endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::EPower;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(AltNum_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedI;
                            else
                    #endif
                                return RepType::INumByDiv;
                #endif
                        return RepType::INum;
                    }
                    break;
            #endif
        #endif
#if !defined(AltNum_UseIntForDecimalHalf)
                default:
                    {
#endif
		#if defined(AltNum_EnableInfinityRep)
                        if(DecimalHalf == InfinityRep)
                            return RepType::Infinity;
		#endif
		#if defined(AltNum_EnableApproachingValues)
                        if (DecimalHalf == ApproachingBottomRep)
                        {
            #if defined(AltNum_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeft;
                            else
            #endif
                                return RepType::ApproachingBottom;
                        }
                        else if (DecimalHalf == ApproachingTopRep)
                        {
            #if defined(AltNum_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidRight;
                            else
            #endif
                            return RepType::ApproachingTop;
                        }
	    #endif
		#if defined(AltNum_EnableNaN)
			            if(DecimalHalf==NaNRep)
				            return RepType::NaN;
			            else if(DecimalHalf==UndefinedRep)
				            return RepType::Undefined;
		#endif
        #if defined(AltNum_EnableNil)
			            if(DecimalHalf==NilRep)
				            return RepType::Nil;
		#endif
		#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            #if defined(AltNum_EnableWithinMinMaxRange)
                        if (ExtraRep == WithinMinMaxRangeRep)
                            return RepType::WithinMinMaxRange;
            #endif
			            if(DecimalHalf==UndefinedInRangeRep)
				            //If IntValue equals 0, than equals undefined value with range between negative infinity and positive infinity 
                            //Otherwise, indicates either negative or positive infinity (outside range of real number representation)
                            return RepType::UndefinedButInRange;
		#endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::ToPowerOf;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(AltNum_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedFrac;
                            else
                    #endif
                                return RepType::NumByDiv;
                #endif
                        return RepType::NormalType;
#if !defined(AltNum_UseIntForDecimalHalf)
                    }
                    break;
            }
#endif
			throw "Unknown or non-enabled representation type detected";//Should not reach this point when code is fully working
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
        virtual void SetPiVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            ExtraRep = Value.ExtraRep;
        #endif
        }

        virtual void SetPiVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            ExtraRep = Value.ExtraRep;
        #endif
        }

protected:
        template<MediumDecVariant VariantType=AltDecBase>
        void SetPiValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
        }

public:
        constexpr auto SetPiVal = SetPiValV0<AltDecBase>;

        virtual void SetPiValFromInt(const int& Value)
        {
        #if defined(AltNum_EnableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
            #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
            #else
            DecimalHalf = PartialInt(0,1);
            ExtraRep = 0;
            #endif
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(AltNum_EnableERep)
        virtual void SetEVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            ExtraRep = Value.ExtraRep;
        #endif
        }

        virtual void SetEVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            ExtraRep = Value.ExtraRep;
        #endif
        }

protected:
        template<MediumDecVariant VariantType=AltDecBase>
        void SetEValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
        }

public:
        constexpr auto SetEVal = SetEValV0<AltDecBase>;

        virtual void SetEValFromInt(const int& Value)
        {
        #if defined(AltNum_EnableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
            #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
            #else
            DecimalHalf = PartialInt(0,2);
            ExtraRep = 0;
            #endif
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(AltNum_EnableIRep)
        virtual void SetIVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = IRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
            ExtraRep = Value.ExtraRep;
        #endif
        }

        virtual void SetIVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = IRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
            ExtraRep = Value.ExtraRep;
        #endif
        }

protected:
        template<MediumDecVariant VariantType=AltDecBase>
        void SetIValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = IRep;
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
        }

public:
        constexpr auto SetIVal = SetIValV0<AltDecBase>;

        virtual void SetIValFromInt(const int& Value)
        {
        #if defined(AltNum_InableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
            #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = IRep;
            #else
            DecimalHalf = PartialInt(0,3);
            ExtraRep = 0;
            #endif
        }
    #endif
    #pragma endregion INum Setters

    #pragma region Fractional Setters
    #if defined(AltNum_EnableFractionals)
        //Set value for NumByDiv
        void SetFractionalVal(int intHalf, int decHalf, int divisor)
        {
            IntValue = intHalf; DecimalHalf = decHalf;
            ExtraRep = divisor;
        }
        
        //Set value for NumByDiv
        void SetFractionalVal(AltDecBase Value, int divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = divisor;
        }

        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetFractionalVal(MediumDec Value, int divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = divisor;
        }
        #endif
        
        //Set value for NumByDiv
        void SetFractionalVal(int Value, int divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = divisor;
        }

        #if defined(AltNum_EnablePiRep)
        //Set value for PiNumByDiv
        void SetDecimaledPiFractional(int intHalf, int decHalf, int divisor)
        {
            IntValue = intHalf; DecimalHalf = PartialInt(decHalf,1);
            ExtraRep = divisor;
        }
	    #endif

        #if defined(AltNum_EnableERep)
        //Set value for ENumByDiv
        void SetDecimaledEFractional(int intHalf, int decHalf, int divisor)
        {
            IntValue = Value; DecimalHalf = PartialInt(decHalf,2);
            ExtraRep = divisor;
        }
	    #endif

        #if defined(AltNum_EnableImaginaryNum)
        //Set value for INumByDiv
        void SetDecimaledIFractional(int intHalf, int decHalf, int divisor)
        {
            IntValue = Value; DecimalHalf = PartialInt(decHalf,3);
            ExtraRep = divisor;
        }
        #endif

    #endif

    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
    #if defined(AltNum_EnableMixedFractional)

        void SetMixedFractionalVal(IntHalfType WholeNum, unsigned int Numerator, unsigned int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = Numerator;
            ExtraRep = MirroredInt(Denom);
        }
		
		#if defined(AltNum_EnablePiRep)
        void SetMixedPiFractionalVal(IntHalfType WholeNum, unsigned int Numerator, unsigned int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = PartialInt(Numerator,1);
            ExtraRep = MirroredInt(Denom);
        }
		#endif
		
		#if defined(AltNum_EnableERep)
        void SetMixedEFractionalVal(IntHalfType WholeNum, unsigned int Numerator, unsigned int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = PartialInt(Numerator,2);
            ExtraRep = MirroredInt(Denom);
        }
		#endif
		
		#if defined(AltNum_EnableImaginaryNum)
        void SetMixedIFractionalVal(IntHalfType WholeNum, unsigned int Numerator, unsigned int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = PartialInt(Numerator,3);
            ExtraRep = MirroredInt(Denom);
        }
		#endif
		
	#endif
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
             ExtraRep = divisor;
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
	#if defined(AltNum_EnablePiRep)

        constexpr auto ConvertPiToNum = MediumDecV2Base::ConvertPiToNum;
    
        void ConvertPiPowerToNum();

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType PiPowerNum(int powerExponent)
        {
	        ExtraRep = 0;
	        MediumDecV2Base PiSide = PiNum;
	        PiSide.IntPowOp(powerExponent);
	        return PiSide;
        }

        void ConvertPiPowerToPiRep();

        virtual void ConvertToPiRep(RepType repType)
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
                case RepType::PiNumByDiv://  (Value/(ExtraRep.Value))*Pi Representation
                {
                    BasicUIntDivOp(ExtraRep.Value);
                }
        #else
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

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType ConvertAsPiRep(RepType repType)
        {
            VariantType convertedVal = *this;
            convertedVal.ConvertToPiRep();
            return convertedVal;
        }

    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	#if defined(AltNum_EnablePiRep)

        constexpr auto ConvertPiToNum = MediumDecV2Base::ConvertPiToNum;
    
    #endif
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
    #if defined(AltNum_EnableMixedFractional)
		//Assumes NormalRep + Normal MixedFraction operation
		void BasicMixedFracAddOp(AltDec& rValue)
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
				AltDec RightSideNum = AltDec(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
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
		
    #if defined(AltNum_UseIntForDecimalHalf)
		virtual void BasicMixedAltFracAddOp(AltDec& rValue)
    #else
		virtual void BasicMixedAltFracAddOp(AltDec& rValue, const AltDec& altNum)
    #endif
		{
			AltDec RightSideNum = AltDec(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
		#if defined(AltNum_UseIntForDecimalHalf)
            #if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		    #else
			RightSideNum *= ENum;
		    #endif
        #else
			RightSideNum *= altNum;
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
		
		//Assumes NormalRep - Normal MixedFraction operation
		virtual void BasicMixedFracSubOp(AltDec& rValue)
		{
		#if defined(AltNum_UseIntForDecimalHalf)
			AltDec RightSideNum = AltDec(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
		#else

        #endif
        	BasicMultOp(rValue.ExtraRep);
			BasicSubOp(RightSideNum);
			if(DecimalHalf==0)
			{
				if(IntValue!=0)//Set as Zero if both are zero
				{
            #if defined(AltNum_UseIntForDecimalHalf)
					DecimalHalf = -DecimalHalf;
					ExtraRep.Value = rValue.ExtraRep.Value;
            #else
					ExtraRep.Value = rValue.ExtraRep.Value;
            #endif
                    ExtraRep.IsPositive = 0;
				}
			}
			else
			{
            #if defined(AltNum_UseIntForDecimalHalf)
                #if defined(AltNum_EnableMirroredSection)
				if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                #else
                if(IntValue.Value!=0)
                #endif
					DecimalHalf = -DecimalHalf;
                ExtraRep = rValue.ExtraRep;
            #else

            #endif
			}
		}
	
    #if defined(AltNum_UseIntForDecimalHalf)
		virtual void BasicMixedAltFracSubOp(AltDec& rValue)
    #else
		virtual void BasicMixedAltFracSubOp(AltDec& rValue, const AltDec& altNum)
    #endif
		{
		#if defined(AltNum_UseIntForDecimalHalf)
			AltDec RightSideNum = AltDec(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
        #else

        #endif
		#if defined(AltNum_UseIntForDecimalHalf)
            #if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		    #else
			RightSideNum *= ENum;
		    #endif
        #else
			RightSideNum *= altNum;
        #endif
			BasicMultOp(rValue.ExtraRep.Value);
			BasicSubOp(RightSideNum);
			if(DecimalHalf==0)
			{
				if(IntValue!=0)//Set as Zero if both are zero
				{
            #if defined(AltNum_UseIntForDecimalHalf)
					DecimalHalf = -DecimalHalf;
					ExtraRep.Value = rValue.ExtraRep.Value;
            #else
					ExtraRep.Value = rValue.ExtraRep.Value;
            #endif
                    ExtraRep.IsPositive = 0;
				}
			}
			else
			{
            #if defined(AltNum_UseIntForDecimalHalf)
                #if defined(AltNum_EnableMirroredSection)
				if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                #else
                if(IntValue.Value!=0)
                #endif
					DecimalHalf = -DecimalHalf;
                ExtraRep.IsPositive = 0;
            #else
            #endif
			}
		}
    #endif
#endif
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
