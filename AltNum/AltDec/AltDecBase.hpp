// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

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
protected:
        #pragma region DigitStorage

        /// <summary>
        /// (Used exclusively for alternative represents of numbers including imaginary numbers and for fractionals)
        /// If ExtraRep>0 and DecimalHalf.Value<AlternativeFractionalLowerBound(and ExtraRep.Value<=FractionalMaximum), then ExtraRep acts as denominator
        /// If ExtraRep is Negative and DecimalHalf.Value<AlternativeFractionalLowerBound, then AltDecBase represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf.Value<999999999, then AltDecBase represents +- 2147483647.999999999
        /// </summary>
        MirroredInt ExtraRep;

        #pragma region DigitStorage

public:
        #pragma region class_constructors
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

        #pragma endregion class_constructors

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
        static const unsigned int ApproachingBottomDivRep = 1073741817;
		//When DecimalHalf.Value equals this value, it represents Approaching IntValue from left towards right (IntValue.9..9)/ExtraRep.Value
		static const unsigned int ApproachingTopDivRep = 1073741816;
	#endif
    #if defined(AltNum_EnableWithinMinMaxRange)
        //Undefined but in ranged of IntValue to DecimalHalf when at this ExtraRep.Value
        static const unsigned int WithinMinMaxRangeRep = 2147483648;
    #endif
        static const unsigned int AlternativeFractionalLowerBound = 1073741816;
        //Maximum divisor for Fractionals and mixed Fractions
        static const unsigned int FractionalDivisorLimit = 1073741815;
	//Fractional Division Maximum at this ExtraRep.Value
        static const unsigned int FractionalMaximum = 2147483648;
	
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
    #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetPiRepType()
    #else
        RepType GetPiRepType()
    #endif
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

#if defined(AltNum_EnableERep)
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
    #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetERepType()
    #else
        RepType GetERepType()
    #endif
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
#endif

#if defined(AltNum_EnableIRep)
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
    #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetIRepType()
    #else
        RepType GetIRepType()
    #endif
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
#endif

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
    #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetNormRepType()
    #else
        RepType GetNormRepType()
    #endif
        {
        #if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf == InfinityRep)
                return RepType::Infinity;
        #endif
        #if defined(AltNum_EnableApproaching)
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
        }

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
#if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetRepType()
#else
        RepType GetRepType()
#endif
        {
            switch(DecimalHalf.Flag)
            {
		#if defined(AltNum_EnablePiRep)
                case 1:
                    return GetPiRepType(); break;
        #endif
		#if defined(AltNum_EnableERep)
                case 2:
                    return GetERepType(); break;
        #endif
		#if defined(AltNum_EnableIRep)
                case 3:
                    return GetIRepType(); break;
        #endif
                default:
                    return GetNormRepType(); break;
            }
        }

#if defined(AltNum_UseBuiltinVirtualTable)

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetPiRepType()
		{
			GetVTable(VTable)->VirtualTable_GetPiRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetERepType()
		{
			GetVTable(VTable)->VirtualTable_GetERepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetIRepType()
		{
			GetVTable(VTable)->VirtualTable_GetIRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetNormRepType()
		{
			GetVTable(VTable)->VirtualTable_GetNormRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetRepType()
		{
			GetVTable(VTable)->VirtualTable_GetRepType(VTable);
		}

#endif

    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMaximum()
        {
            IntValue = MaxIntValue; DecimalHalf = 999999999; ExtraRep = 0;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
            IntValue = MinIntValue; DecimalHalf = 999999999; ExtraRep = 0;
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
        void SetPiValV1(const VariantType& Value)
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
        constexpr auto SetPiVal = SetPiValV1<AltDecBase>;

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
        void SetEValV1(const VariantType& Value)
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
        constexpr auto SetEVal = SetEValV1<AltDecBase>;

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
        void SetIValV1(const VariantType& Value)
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

        #if defined(AltNum_EnableIRep)
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
		
		#if defined(AltNum_EnableIRep)
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
	#if defined(AltNum_EnableApproaching)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottom(const int& value=0)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
            ExtraRep = 0;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTop(const int& value=0)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = 0;
        }
        #endif
		
		#if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPi(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
            ExtraRep = 0;
        }
		
        void SetAsApproachingBottomPi(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,1);
            ExtraRep = 0;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopE(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
            ExtraRep = 0;
        }
		
        void SetAsApproachingBottomE(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,2);
            ExtraRep = 0;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopI(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
            ExtraRep = 0;
        }
		
        void SetAsApproachingBottomI(const int& value=0)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,3);
            ExtraRep = 0;
        }
		#endif
    #endif
    //Separating into separate methods since second parameter not same as previous virtual function
    #if defined(AltNum_EnableApproachingDivided)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottomDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
            ExtraRep = divisor;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTopDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = divisor;
        }
        #endif
		
		#if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPiDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
            ExtraRep = divisor;
        }
		
        void SetAsApproachingBottomPiDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,1);
            ExtraRep = divisor;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopEDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
            ExtraRep = divisor;
        }
		
        void SetAsApproachingBottomEDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
            ExtraRep = divisor;
        }
		#endif
		
		#if defined(AltNum_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopIDiv(const int& value, const unsigned int& divisor)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
            ExtraRep = divisor;
        }
		
        void SetAsApproachingBottomIDiv(const int& value, const unsigned int& divisor)
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
	
protected:

        void InitialyzeAltRepFromString(const std::string& Value)
        {
        #if defined(AltNum_EnablePiRep)
            if(str.find("Pi") != std::string::npos)
            #if !defined(AltNum_UseIntForDecimalHalf)
                DecimalHalf.Flags = 1;
            #else
                ExtraRep = PiRep;
            #endif
        #endif
        #if defined(AltNum_EnableERep)
            if(Value.last()=='e')
            #if !defined(AltNum_UseIntForDecimalHalf)
                DecimalHalf.Flags = 2;
            #else
                ExtraRep = ERep;
            #endif
        #endif
        #if defined(AltNum_EnableIRep)
            if(Value.last()=='i')
            #if !defined(AltNum_UseIntForDecimalHalf)
                DecimalHalf.Flags = 3;
            #else
                ExtraRep = IRep;
            #endif
        #endif
        }

public:

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        constexpr auto ReadString = MediumDecV2Base::ReadString;

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecV2Base</returns>
        constexpr auto GetValueFromString = MediumDecBase::GetValueFromString<AltDecBase>;

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        AltDecBase(const char* strVal)
        {
            std::string Value = strVal;
            ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDecBase(const std::string& Value)
        {
            ReadString(Value);
        }
        #pragma endregion String Commands

        #pragma region ConvertFromOtherTypes
		
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& Value)
        {
		#if defined(AltNum_EnableMixedFractional)
			//Can be converted at 100% precision from float after extracting "2^Exp + SignifNum*(2^(Exp - 23))" format information from bits of float
		#else
			MediumDec::SetFloatVal(Value);
			ExtraRep = 0;
		#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDoubleVal(const double& Value)
        {
			MediumDec::SetDoubleVal(Value);
			ExtraRep = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDecimalVal(const ldouble& Value)
        {
			MediumDec::SetDecimalVal(Value);
			ExtraRep = 0;
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetBoolVal(const bool& Value)
        {
			MediumDec::SetBoolVal(Value);
			ExtraRep = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntVal(const int& Value)
        {
			MediumDec::SetIntVal(Value);
			ExtraRep = 0;
        }
		
        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDecBase(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDecBase(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDecBase(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDecBase"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDecBase(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableAltDecBaseBasedSetValues)
        AltDecBase(const AltDecBase& Value)
        {
            this->SetVal(Value);
        }
#endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble() { return toDecimal(); }

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
	#if defined(AltNum_EnablePiRep)

        constexpr auto ConvertPiToNum = MediumDecV2Base::ConvertPiToNum;
    
        #if defined(AltNum_EnablePowerOfRepresentation)
        void ConvertPiPowerToNum();

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType PiPowerNum(int powerExponent)
        {
	        ExtraRep = 0;
	        auto PiSide = PiNum;
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
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep.Value))*Pi Representation
                    {
                        BasicUIntDivOp(ExtraRep.Value);
        #if defined(AltNum_UseIntForDecimalHalf)
                        ExtraRep = PiRep;
        #else
                        ExtraRep.Value = 0;
        #endif
                    }
                    break;

    #endif
                default:
                    break;
            }
        }

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType ConvertAsPiRep(RepType repType)
        {
            VariantType convertedVal = *this;
            convertedVal.ConvertToPiRep();
            return convertedVal;
        }
        #endif

    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	#if defined(AltNum_EnableERep)

        constexpr auto ConvertEToNum = MediumDecV2Base::ConvertEToNum;
    
        #if defined(AltNum_EnablePowerOfRepresentation)
        void ConvertEPowerToNum();

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType EPowerNum(int powerExponent)
        {
	        ExtraRep = 0;
	        auto ESide = ENum;
	        ESide.IntPowOp(powerExponent);
	        return ESide;
        }

        void ConvertEPowerToERep();

        virtual void ConvertToERep(RepType repType)
        {
            switch (repType)
            {
                case RepType::ENum:
                    return;
                    break;
    #if defined(AltNum_EnableEPowers)
                case RepType::EPower:
                    ConvertEPowerToERep();
                    break;
    #endif
    #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://  (Value/(ExtraRep.Value))*E Representation
                    {
                        BasicUIntDivOp(ExtraRep.Value);
        #if defined(AltNum_UseIntForDecimalHalf)
                        ExtraRep = ERep;
        #else
                        ExtraRep.Value = 0;
        #endif
                    }
                    break;

    #endif
                default:
                    break;
            }
        }

        template<MediumDecVariant VariantType=AltDecBase>
        VariantType ConvertAsERep(RepType repType)
        {
            VariantType convertedVal = *this;
            convertedVal.ConvertToERep();
            return convertedVal;
        }
        #endif

    #endif
    #pragma endregion E Conversion

    #pragma region Imaginary Conversion
    #if defined(AltNum_EnableIRep)


    #endif
    #pragma endregion region Imaginary Conversion

    #pragma region Other RepType Conversion
    #if defined(AltNum_EnableApproachingDivided)

    #endif

        //Returns value as normal type or INum representation
        void ConvertToNormType(const RepType& repType)
        {
            switch (repType)
            {
            case RepType::NormalType:
                break;
	#if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
                break;
	#endif
	#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                ConvertPiToNum(); break;
		#if defined(AltNum_EnableFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                ConvertFromPiByDivToNorm(); break;
		#endif
		#if defined(AltNum_EnableApproaching)
            case RepType::ApproachingBottomPi:
                DecimalHalf.Value = 1;
                break;
			#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTopPi:
                DecimalHalf.Value = 999999999;
                break;
			#endif
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
            case RepType::PiPower:
                ConvertPiPowerToNum(); break;
		#endif
	#endif
	#if defined(AltNum_EnableERep)
            case RepType::ENum:
                ConvertENumToNum(); break;
		#if defined(AltNum_EnableFractionals)
            case RepType::ENumByDiv:
                ConvertFromEByDivToNorm(); break;
		#endif
		#if defined(AltNum_EnableApproaching)
            case RepType::ApproachingBottomPi:
                DecimalHalf.Value = 1;
                break;
			#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTopPi:
                DecimalHalf.Value = 999999999;
                break;
			#endif
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
            case RepType::EPower:
                ConvertEPowerToNum(); break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::Infinity:
				IntValue = IsPositive()?MaxIntValue:MinIntValue; 
				DecimalHalf = 999999999;
				/*ExtraRep = 0;*/
				break;
	#endif
	#if defined(AltNum_EnableApproaching)
            case RepType::ApproachingBottom:
                DecimalHalf = 1;
                break;
		#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
                DecimalHalf = 999999999;
                break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                ConvertFromApproachingMidLeftToNorm(); break;
			#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                ConvertFromApproachingMidRightToNorm(); break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            {
                auto Res = AltDecBase(DecimalHalf.Value, 0);
                Res.BasicUIntDivOp(ExtraRep.Value);
                if (IntValue.Value!=0)
                    Res += IntValue;
                IntValue = Res.IntValue;
                DecimalHalf.Value = Res.DecimalHalf;
                ExtraRep = 0;
            }
			break;
		#if defined(AltNum_EnablePiRep)
            case RepType::MixedPi:
            {
                auto Res = AltDecBase(DecimalHalf.Value, PartialInt(0,1));
                Res.BasicUIntDivOp(ExtraRep.Value);
                if (IntValue.Value!=0)
                    Res += IntValue;
                IntValue = Res.IntValue;
                DecimalHalf.Value = Res.DecimalHalf;
                ExtraRep = 0;
            }
			break;
		#endif
		#if defined(AltNum_EnableERep)
            case RepType::MixedPi:
            {
                auto Res = AltDecBase(DecimalHalf.Value, PartialInt(0,2));
                Res.BasicUIntDivOp(ExtraRep.Value);
                if (IntValue.Value!=0)
                    Res += IntValue;
                IntValue = Res.IntValue;
                DecimalHalf.Value = Res.DecimalHalf;
                ExtraRep = 0;
            }
			break;
		#endif
		#if defined(AltNum_EnableIRep)
            case RepType::MixedPi:
            {
                auto Res = AltDecBase(DecimalHalf.Value, PartialInt(0,3));
                Res.BasicUIntDivOp(ExtraRep.Value);
                if (IntValue.Value!=0)
                    Res += IntValue;
                IntValue = Res.IntValue;
                DecimalHalf.Value = Res.DecimalHalf;
                ExtraRep = 0;
            }
			break;
		#endif
	#endif
	#if defined(AltNum_EnableIRep)
			case RepType::INum:
				break;

		#if defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/(ExtraRep.Value))*i Representation
					BasicUnsignedDivOp(ExtraRep.Value);
					ExtraRep = 0;
				break;
		#endif
		#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingImaginaryBottom:
				DecimalHalf.Value = 1;
				break;
			#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingImaginaryTop:
				DecimalHalf.Value = 999999999;
				break;
			#endif
			#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingImaginaryMidLeft:
				ConvertFromApproachingIMidLeftToNorm(); break;
				#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingImaginaryMidRight:
				ConvertFromApproachingIMidRightToNorm(); break;
				#endif
			#endif
		#endif
		#if defined(AltNum_EnableInfinityRep)
			case RepType::ImaginaryInfinity:
				IntValue = IsPositive()?MaxIntValue:MinIntValue; 
				DecimalHalf.Value = 999999999;
				ExtraRep = 0;
				break;
		#endif
		#ifdef AltNum_EnableComplexNumbers
			case RepType::ComplexIRep:
				throw "Conversion from complex number to real number not supported yet.";
				break;
		#endif
	#endif
            default:
                throw "Conversion to normal number not supported yet?";
                break;
            }
        } const

		//Returns value as normal type or INum representation
        auto ConvertAsNormType(const RepType& repType)
        {
            auto Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        }

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }

		//Returns value as normal type representation
        auto ConvertAsNormTypeV2()
        {
            VariantType Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }

    #pragma endregion Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
		//Converts Representation down to basic PiNum,ENum,INum, and NormalType representations 
		virtual void ConvertDownToMediumDecV2Equiv()
		{
	#if defined(AltNum_UseIntForDecimalHalf)
			//To-Do:Add code to convert down to base PiNum,ENum,INum, and NormalType
	#else
		#if defined(AltNum_EnableMixedFractional)
			if(ExtraRep.IsNegative())
			{
				//To-Do add code here to convert from mixed fraction
				return;
			}
		#elif defined(AltNum_EnablePowerOfRepresentation)
			#if defined(AltNum_EnableNegativePowerRep)
			if(ExtraRep!=0)
			#else
			if(ExtraRep.IsNegative())
			#endif
			{
				if(DecimalHalf.Flag==1)//Convert down to PiNum
					ConvertPiPowerToPiRep();
				else if(DecimalHalf.Flag==2)//Convert down to ENum
					ConvertEPowerToERep();
				return;
			}
		#endif
		#if defined(AltNum_EnableFractionals)
			if(ExtraRep.Value!=0)
			{
				BasicIntDivOp(ExtraRep.Value);
				ExtraRep.Value = 0;
				return;
			}
		#endif
			ConvertToNormTypeV2();
	#endif
		}

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
	
public:
		//Unsigned Templated version of Spaceship operator without certain feature support
		//Designed for use when dividing by 2 constantly to simplify the comparison(Acts as simplified spaceship operator)
		std::strong_ordering UnsignedSimpleCompareWith(const auto that) const
		{
	#if defined(AltNum_UseIntForDecimalHalf)
		//To-Do add code here
	#else
		#if defined(AltNum_EnableFractionals)
			if(ExtraRep!=0)//RepType:NumByDiv Assumed
			{
				auto lSide = *this;
				lSide.BasicIntDivOp(ExtraRep.Value);
				auto rSide = that;
				if(that.ExtraRep!=0)
					rSide.BasicIntDivOp(that.ExtraRep.Value);
#if defined(AltNum_EnableMirroredSection)
				return BasicComparisonV2(rSide);
#else
				return BasicComparison(rSide);
#endif
			}
			else if(that.ExtraRep!=0)//RepType:NumByDiv Assumed
			{
				auto rSide = that;
				rSide.BasicIntDivOp(that.ExtraRep.Value);
#if defined(AltNum_EnableMirroredSection)
				return BasicComparisonV2(rSide);
#else
				return BasicComparison(rSide);
#endif
			}
			else//Otherwise, assume NormalType
		#endif
	#if defined(AltNum_EnableMirroredSection)
				return BasicComparisonV2(rSide);
	#else
				return BasicComparison(rSide);
	#endif
		}

protected:
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
		#if defined(AltNum_EnableIRep)
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
	#if defined(AltNum_EnableApproaching)
	
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
						auto lSide = *this;
						auto rSide = that;
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
		template<MediumDecVariant VariantType=AltDecBase>
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
	#if defined(AltNum_EnableIRep
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
				auto lSide = *this;
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
		std::strong_ordering operator<=>(const AltDecBase& that) const
		{
			return CompareWith(that);
		}

	/*  
		//Add comparisons to previous parent classes with more limited ranges based on supported values

    */

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
		
		bool operator==(const MediumDec& that) const
		{
		#if defined(AltNum_EnableUndefinedButInRange)
			if(DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
			else if(that.DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
            #if defined(AltNum_EnableWithinMinMaxRange)
                //ToDo:Add comparison code for comparing unknown number within range
            #endif
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
			AltDec LValue = this;
			LValue.ConvertToNormTypeV2();
			MediumDec RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			AltDec LValue = this;
			MediumDec RValue = that;
			if(DecimalHalf.Flags!=0)
				return false;
		#endif
			if (LValue.IntValue!=RValue.IntValue)
				return false;
			if (LValue.DecimalHalf!=RValue.IntValue)
				return false;
		}
		
		bool operator==(const MediumDecV2& that) const
		{
		#if defined(AltNum_EnableUndefinedButInRange)
			if(DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
			else if(that.DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
            #if defined(AltNum_EnableWithinMinMaxRange)
                //ToDo:Add comparison code for comparing unknown number within range
            #endif
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
			AltDec LValue = this;
			LValue.ConvertToNormTypeV2();
			MediumDecV2 RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			AltDec LValue = this;
			MediumDecV2 RValue = that;
			if(DecimalHalf.Flags==RValue.DecimalHalf.Flags)
				LValue.ConvertDownToMediumDecV2Equiv();
			else if((DecimalHalf.Flags==3 && RValue.DecimalHalf.Flags!=3)||(RValue.DecimalHalf.Flags==3 && LValue.DecimalHalf.Flags!=3))
				throw "Can't compare imaginary number with real number";
			else if(DecimalHalf.Flags!=0)
				return false;
		#endif
			if (LValue.IntValue!=RValue.IntValue)
				return false;
			if (LValue.DecimalHalf!=RValue.IntValue)
				return false;
		}
		
		bool operator==(const AltDec& that) const
		{
		#if defined(AltNum_EnableUndefinedButInRange)
			if(DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
			else if(that.DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
            #if defined(AltNum_EnableWithinMinMaxRange)
                //ToDo:Add comparison code for comparing unknown number within range
            #endif
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
			AltDec LValue = this;
			LValue.ConvertToNormTypeV2();
			AltDec RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			AltDec LValue = this;
			AltDec RValue = that;
			if(DecimalHalf.Flags==RValue.DecimalHalf.Flags)
            {
				LValue.ConvertDownToMediumDecV2Equiv();
                RValue.ConvertDownToMediumDecV2Equiv();
            }
			else if((DecimalHalf.Flags==3 && RValue.DecimalHalf.Flags!=3)||(RValue.DecimalHalf.Flags==3 && LValue.DecimalHalf.Flags!=3))
				throw "Can't compare imaginary number with real number";
			else
				return false;
		#endif
			if (LValue.IntValue!=RValue.IntValue)
				return false;
			if (LValue.DecimalHalf!=RValue.IntValue)
				return false;
		}

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer division operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOp = MediumDecBase::PartialUIntDivOp<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOp = MediumDecBase::PartialIntDivOp<IntType>;

public:

        /// <summary>
        /// Basic division operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOp = MediumDecBase::BasicUIntDivOp<IntType>;

        /// <summary>
        /// Basic division operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOp = MediumDecBase::BasicIntDivOp<IntType>;

    #pragma endregion NormalRep Integer division operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOp = MediumDecBase::PartialIntMultOp<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialUIntMultOp = MediumDecBase::PartialUIntMultOp<IntType>;

public:
        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOp = MediumDecBase::BasicIntMultOp<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicUIntMultOp = MediumDecBase::BasicUIntMultOp<IntType>;

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOp = MediumDecBase::BasicIntAddOp<AltDecBase>;

	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

        /// <summary>
        /// Basic subtraction operation between AltDec and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOp = MediumDecBase::BasicIntSubOp<AltDecBase>;

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations
    //To-Do:Need to update this code
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

    #pragma region NormalRep AltNum division operations

    #pragma endregion NormalRep AltNum division operations

    #pragma region NormalRep AltNum Multiplication Operations

    #pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other division operations

    #if defined(AltNum_EnableFractionals)

		//Simplified division by 2 operation(to reduce cost of dividing)
        void DivideByTwo()
        {
			//To-Do:Adjust code for including power of and mixed fractions
            if(ExtraRep==0)
                ExtraRep = 2;
            else if(ExtraRep<=1073741823)
                ExtraRep *= 2;
			else if(DecimalHalf==0&&IntValue.Value&1==0)//Check if even whole number
				IntValue /= 2;
            else
            {
                BasicIntDivOp(65536);//Divided by 2^16
                ExtraRep /= 32768;//Divided by 2^16, and then multiplied by 2
            }
        }

		//Simplified division by 4 operation(to reduce cost of multiplication)
        void DivideByFour()
        {
            if(ExtraRep==0)
                ExtraRep = 4;
            else if(ExtraRep<=1073741824)
                ExtraRep *= 4;
            else
            {
                rValue.UnsignedBasicIntDivOp(65536);//Divided by 2^16
                ExtraRep /= 16384;//Divided by 2^16, and then multiplied by 2
            }       
        }

    #else

		//Simplified division by 2 operation(to reduce cost of dividing)
        constexpr auto DivideByTwo = MediumDecBase::DivideByTwo;

		//Simplified division by 4 operation(to reduce cost of dividing)
        constexpr auto DivideByFour = MediumDecBase::DivideByFour;

    #endif

protected:
        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= unsigned int>
        auto UIntDivOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)&&defined(AltNum_DefineDivideByZeroAsInfinity)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    switch(LRep)
                    {
                        case RepType::PiNum:{
                            #if defined(AltNum_EnableFractionals)
                            ExtraRep = rValue;
                            #else
                            BasicUIntDivOp(rValue);
                            #endif
                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::PiNumByDiv:
                        {
                            unsigned int result = ExtraRep.Value * rValue;
                            if (ExtraRep.Value == result / rValue)//checking for overflow
                                ExtraRep = result;
                            else
                                BasicUIntDivOp(rValue);
                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedPi://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            unsigned int divRes;
                            unsigned int C;
                            if (IsAtZeroInt())//Become NumByDiv
                            {
                                divRes = DecimalHalf.Value / ExtraRep.Value;
                                C = DecimalHalf.Value - ExtraRep.Value * divRes;
                                if (C == 0)
                                {
                                    IntValue = divRes;
                                    DecimalHalf = 0;
                                }
                                else
                                {
                                    IntValue.Value = DecimalHalf.Value;
                                    DecimalHalf = 0;
                                    ExtraRep *= rValue;
                                }
                                return *this;
                            }
							else
							{
								//(2+3/4)/6 == (2/6+3/24)
								//(1/3+2/24) == (1*3/3+2*3/24)
								//3+6/24 == 3+1/4
								divRes = IntValue / rValue;
								if (divRes != 0)
								{
									C = IntValue - rValue * divRes;
									if (C == 0)
									{
										IntValue.Value = divRes;
										ExtraRep.Value *= rValue;
									}
									else
									{
										//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
										boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
										IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
										boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
										DecimalHalf.Value = rSideFrac.numerator();
										ExtraRep.Value = rSideFrac.denominator();
									}
								}
								else
								{
									//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
									boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
									IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
									boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
									DecimalHalf.Value = rSideFrac.numerator();
									ExtraRep.Value = rSideFrac.denominator();
								}
							}
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomPi://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopPi://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightPi://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftPi://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    switch(LRep)
                    {
                        case RepType::ENum:{
                            #if defined(AltNum_EnableFractionals)
                            ExtraRep = rValue;
                            #else
                            BasicUIntDivOp(rValue);
                            #endif
                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::ENumByDiv:
                        {
                            unsigned int result = ExtraRep.Value * rValue;
                            if (ExtraRep.Value == result / rValue)//checking for overflow
                                ExtraRep = result;
                            else
                                BasicUIntDivOp(rValue);
                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedE://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            unsigned int divRes;
                            unsigned int C;
                            if (IsAtZeroInt())//Become NumByDiv
                            {
                                divRes = DecimalHalf.Value / ExtraRep.Value;
                                C = DecimalHalf.Value - ExtraRep.Value * divRes;
                                if (C == 0)
                                {
                                    IntValue = divRes;
                                    DecimalHalf = 0;
                                }
                                else
                                {
                                    IntValue.Value = DecimalHalf.Value;
                                    DecimalHalf = 0;
                                    ExtraRep *= rValue;
                                }
                                return *this;
                            }
							else
							{
								//(2+3/4)/6 == (2/6+3/24)
								//(1/3+2/24) == (1*3/3+2*3/24)
								//3+6/24 == 3+1/4
								divRes = IntValue / rValue;
								if (divRes != 0)
								{
									C = IntValue - rValue * divRes;
									if (C == 0)
									{
										IntValue.Value = divRes;
										ExtraRep.Value *= rValue;
									}
									else
									{
										//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
										boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
										IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
										boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
										DecimalHalf.Value = rSideFrac.numerator();
										ExtraRep.Value = rSideFrac.denominator();
									}
								}
								else
								{
									//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
									boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
									IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
									boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
									DecimalHalf.Value = rSideFrac.numerator();
									ExtraRep.Value = rSideFrac.denominator();
								}
							}
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomE://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopE://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightE://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftE://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:{
                    RepType LRep = rValue.GetIRepType();
                    switch(LRep){
                        case RepType::INum:{
                            #if defined(AltNum_EnableFractionals)
                            ExtraRep = rValue;
                            #else
                            BasicUIntDivOp(rValue);
                            #endif
                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::INumByDiv:
                        {
                            unsigned int result = ExtraRep.Value * rValue;
                            if (ExtraRep.Value == result / rValue)//checking for overflow
                                ExtraRep = result;
                            else
                                BasicUIntDivOp(rValue);
                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedI://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            unsigned int divRes;
                            unsigned int C;
                            if (IsAtZeroInt())//Become NumByDiv
                            {
                                divRes = DecimalHalf.Value / ExtraRep.Value;
                                C = DecimalHalf.Value - ExtraRep.Value * divRes;
                                if (C == 0)
                                {
                                    IntValue = divRes;
                                    DecimalHalf = 0;
                                }
                                else
                                {
                                    IntValue.Value = DecimalHalf.Value;
                                    DecimalHalf = 0;
                                    ExtraRep *= rValue;
                                }
                                return *this;
                            }
							else
							{
								//(2+3/4)/6 == (2/6+3/24)
								//(1/3+2/24) == (1*3/3+2*3/24)
								//3+6/24 == 3+1/4
								divRes = IntValue / rValue;
								if (divRes != 0)
								{
									C = IntValue - rValue * divRes;
									if (C == 0)
									{
										IntValue.Value = divRes;
										ExtraRep.Value *= rValue;
									}
									else
									{
										//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
										boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
										IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
										boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
										DecimalHalf.Value = rSideFrac.numerator();
										ExtraRep.Value = rSideFrac.denominator();
									}
								}
								else
								{
									//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
									boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
									IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
									boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
									DecimalHalf.Value = rSideFrac.numerator();
									ExtraRep.Value = rSideFrac.denominator();
								}
							}
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
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
                            BasicUIntDivOp(rValue);
                        }
                        break;
            #endif
            #if defined(AltNum_EnableImaginaryInfinity)
                        case RepType::ImaginaryInfinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep)
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
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::NumByDiv:
                        {
                            int result = ExtraRep * rValue;
                            if (ExtraRep.Value == result / rValue)//checking for overflow
                                ExtraRep.Value = result;
                            else
                                BasicUIntDivOp(rValue);
                        }
                        break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            unsigned int divRes;
                            unsigned int C;
                            if (IsAtZeroInt())//Become NumByDiv
                            {
                                divRes = DecimalHalf.Value / ExtraRep.Value;
                                C = DecimalHalf.Value - ExtraRep.Value * divRes;
                                if (C == 0)
                                {
                                    IntValue = divRes;
                                    DecimalHalf = 0;
                                }
                                else
                                {
                                    IntValue.Value = DecimalHalf.Value;
                                    DecimalHalf = 0;
                                    ExtraRep *= rValue;
                                }
                                return *this;
                            }
							else
							{
								//(2+3/4)/6 == (2/6+3/24)
								//(1/3+2/24) == (1*3/3+2*3/24)
								//3+6/24 == 3+1/4
								divRes = IntValue / rValue;
								if (divRes != 0)
								{
									C = IntValue - rValue * divRes;
									if (C == 0)
									{
										IntValue.Value = divRes;
										ExtraRep.Value *= rValue;
									}
									else
									{
										//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
										boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
										IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
										boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
										DecimalHalf.Value = rSideFrac.numerator();
										ExtraRep.Value = rSideFrac.denominator();
									}
								}
								else
								{
									//Multiply left side into whole value after division of rValue and then multiple Numerator of right side of mixed fraction by the same amount
									boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
									IntValue.Value = lSideFrac.numerator()*lSideFrac.denominator();
									boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value*lSideFrac.denominator(), ExtraRep.Value * rValue);
									DecimalHalf.Value = rSideFrac.numerator();
									ExtraRep.Value = rSideFrac.denominator();
								}
							}
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
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
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        	}
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= signed int>
        auto IntDivOpV1(const IntType& rValue)
		{
            if(Value<0)
            {
                SwapNegativeStatus();
                UIntDivOpV1(-Value);
            }
            else
                UIntDivOpV1(Value);
		}

        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= unsigned int>
        auto DivByUIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.UIntDivOpV1(rValue);
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivByIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.IntDivOpV1(rValue);
		}

public:

        constexpr auto UIntDivOpV1 = UIntDivOpV1<unsigned int>;
        constexpr auto IntDivOpV1 = IntDivOpV1<signed int>;
        constexpr auto UnsignedIntDivOp = UIntDivOpV1<signed int>;
        constexpr auto UInt64DivOp = UIntDivOpV1<unsigned long long>;
        constexpr auto Int64DivOp = IntDivOpV1<signed long long>;
	
        constexpr auto DivByUInt = DivByUIntV1<unsigned int>;
        constexpr auto DivByInt = DivByIntV1<signed int>;
        constexpr auto UnsignedDivByInt = DivByUInt<signed int>;
        constexpr auto DivByUInt64 = DivByUInt<;
        constexpr auto DivByInt64 = BasicDivByInt64;
        constexpr auto UnsignedDivByInt64 = DivByUInt<signed long long>;
		
        constexpr auto DivByUInt8 = BasicDivByUInt8;
        constexpr auto DivByInt8 = BasicDivByInt8;
        constexpr auto DivByUInt16 = BasicDivByUInt16;
        constexpr auto DivByInt16 = BasicDivByInt16;

public:

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& UnsignedDivOp(const auto& rValue);

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
        /// <returns>AltDecBase</returns>
        friend AltDecBase operator/(const AltDecBase& self, const AltDecBase& Value) { return self.DivideBy(Value); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase& operator/=(AltDecBase& self, const AltDecBase& Value) { return self.DivOp(Value); }
		
        /// <summary>
        /// Division operation between AltDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase operator/(const AltDecBase& self, const signed int& Value) { return self.DivideByInt(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const signed long long& Value) { return self.DivideByInt64(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const unsigned long long& Value) { return self.DivideByUInt64(Value); }
		
        friend AltDecBase operator/(const signed int& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const signed long long& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const unsigned int& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const unsigned long long& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }

        friend AltDecBase operator/(const AltDecBase& self, const signed char& Value) { return self.DivideByInt8(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const signed short& Value) { return self.DivideByInt16(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const unsigned char& Value) { return self.DivideByUInt8(Value); }
        friend AltDecBase operator/(const AltDecBase& self, const unsigned short& Value) { return self.DivideByUInt16(Value); }

        friend AltDecBase operator/(const signed char& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const signed short& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const unsigned char& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }
        friend AltDecBase operator/(const unsigned short& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between AltDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase& operator/=(AltDecBase& self, const signed int& Value) { return self.IntDivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const signed long long& Value) { return self.Int64DivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const unsigned long long& Value) { return self.UInt64DivOp(Value); }

        friend AltDecBase& operator/=(AltDecBase& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend AltDecBase& operator/=(AltDecBase& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }

	#pragma endregion Other division operations	

    #pragma region Other multiplication operations

        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        void MultiplyByTwo()
        {
			if(ExtraRep&1==0)
				ExtraRep /= 2;
			else
				UIntDivOp(2);
        }

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        void MultiplyByFour()
        {
			if(((ExtraRep >> 2) << 2) == ExtraRep))
                ExtraRep /= 4;
			else if(DecimalHalf<
				UIntDivOp(4);
        }

protected:
        /// <summary>
        /// Multiplication operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntMultOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntMultOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto MultByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= signed int>
        auto MultByIntV1(const IntType& rValue)
		{
			//Add Code here
		}


public:

        constexpr auto UIntMultOpV1 = UIntMultOpV1<unsigned int>;
        constexpr auto IntMultOpV1 = IntMultOpV1<signed int>;
        constexpr auto UnsignedIntMultOp = UIntMultOpV1<signed int>;
        constexpr auto UInt64MultOp = UIntMultOpV1<unsigned long long>;
        constexpr auto Int64MultOp = IntMultOpV1<signed long long>;
	
        constexpr auto MultByUInt = MultByUIntV1<unsigned int>;
        constexpr auto MultByInt = MultByIntV1<signed int>;
        constexpr auto UnsignedMultByInt = MultByUInt<signed int>;
        constexpr auto MultByUInt64 = MultByUInt<;
        constexpr auto MultByInt64 = BasicMultByInt64;
        constexpr auto UnsignedMultByInt64 = MultByUInt<signed long long>;
		
        constexpr auto MultByUInt8 = BasicMultByUInt8;
        constexpr auto MultByInt8 = BasicMultByInt8;
        constexpr auto MultByUInt16 = BasicMultByUInt16;
        constexpr auto MultByInt16 = BasicMultByInt16;

        /// <summary>
        /// Unsigned multiplication operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& UnsignedMultOp(const auto& rValue);

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void MultOp(const auto& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedMultOp(Value);
        }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto MultiplyByUnsigned(const auto& rValue)
        {
            auto self = *this;
            return self.UnsignedMultOp(rValue);
        }

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto MultiplyBy(const auto& rValue)
        {
            auto self = *this;
            return self.MultOp(rValue);
        }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase operator*(const AltDecBase& self, const AltDecBase& Value) { return self.MultBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase& operator*=(AltDecBase& self, const AltDecBase& Value) { return self.MultOp(Value); }
		
        /// <summary>
        /// Multition operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase operator*(const AltDecBase& self, const signed int& Value) { return self.MultByInt(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const signed long long& Value) { return self.MultByInt64(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const unsigned int& Value) { return self.MultByUInt(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const unsigned long long& Value) { return self.MultByUInt64(Value); }
		
        friend AltDecBase operator*(const signed int& lValue, const AltDecBase& rValue) { return rValue.MultByInt(lValue); }
        friend AltDecBase operator*(const signed long long& lValue, const AltDecBase& rValue) { return rValue.MultByInt64(lValue); }
        friend AltDecBase operator*(const unsigned int& lValue, const AltDecBase& rValue) { return rValue.MultByUInt(lValue); }
        friend AltDecBase operator*(const unsigned long long& lValue, const AltDecBase& rValue) { return rValue.MultByUInt64(lValue); }

        friend AltDecBase operator*(const AltDecBase& self, const signed char& Value) { return self.MultByInt8(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const signed short& Value) { return self.MultByInt16(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const unsigned char& Value) { return self.MultByUInt8(Value); }
        friend AltDecBase operator*(const AltDecBase& self, const unsigned short& Value) { return self.MultByUInt16(Value); }

        friend AltDecBase operator*(const signed char& lValue, const AltDecBase& rValue) { return rValue.MultByInt8(lValue); }
        friend AltDecBase operator*(const signed short& lValue, const AltDecBase& rValue) { return rValue.MultByInt16(lValue); }
        friend AltDecBase operator*(const unsigned char& lValue, const AltDecBase& rValue) { return rValue.MultByUInt8(lValue); }
        friend AltDecBase operator*(const unsigned short& lValue, const AltDecBase& rValue) { return rValue.MultByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase& operator*=(AltDecBase& self, const signed int& Value) { return self.IntMultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const signed long long& Value) { return self.Int64MultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const unsigned long long& Value) { return self.UInt64MultOp(Value); }

        friend AltDecBase& operator*=(AltDecBase& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend AltDecBase& operator*=(AltDecBase& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

    #pragma endregion Other multiplication operations

    #pragma region Other addition operations
protected:

        /// <summary>
        /// Addition operation between MediumDecBase and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntAddOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntAddOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto AddByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= signed int>
        auto AddByIntV1(const IntType& rValue)
		{
			//Add Code here
		}

public:

        constexpr auto UIntAddOpV1 = UIntAddOpV1<unsigned int>;
        constexpr auto IntAddOpV1 = IntAddOpV1<signed int>;
        constexpr auto UnsignedIntAddOp = UIntAddOpV1<signed int>;
        constexpr auto UInt64AddOp = UIntAddOpV1<unsigned long long>;
        constexpr auto Int64AddOp = IntAddOpV1<signed long long>;
	
        constexpr auto AddByUInt = AddByUIntV1<unsigned int>;
        constexpr auto AddByInt = AddByIntV1<signed int>;
        constexpr auto UnsignedAddByInt = AddByUInt<signed int>;
        constexpr auto AddByUInt64 = AddByUInt<;
        constexpr auto AddByInt64 = BasicAddByInt64;
        constexpr auto UnsignedAddByInt64 = AddByUInt<signed long long>;
		
        constexpr auto AddByUInt8 = BasicAddByUInt8;
        constexpr auto AddByInt8 = BasicAddByInt8;
        constexpr auto AddByUInt16 = BasicAddByUInt16;
        constexpr auto AddByInt16 = BasicAddByInt16;

        /// <summary>
        /// Unsigned Addition operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& UnsignedAddOp(const auto& rValue);

        /// <summary>
        /// Addition operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& AddOp(const auto& Value)
        {
            if(Value<0)
                return UnsignedSubOp(-Value);
            else
                return UnsignedAddOp(Value);
        }

		/// <summary>
        /// Unsigned addition operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto AddByUnsigned(const auto& rValue)
        {
            auto self = *this;
            return self.UnsignedAddOp(rValue);
        }

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto AddBy(const auto& rValue)
        {
            auto self = *this;
            return self.AddOp(rValue);
        }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase operator+(const AltDecBase& self, const AltDecBase& Value) { return self.AddBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase& operator+=(AltDecBase& self, const AltDecBase& Value) { return self.AddOp(Value); }
		
        /// <summary>
        /// Addition operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase operator+(const AltDecBase& self, const signed int& Value) { return self.AddByInt(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const signed long long& Value) { return self.AddByInt64(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const unsigned long long& Value) { return self.AddByUInt64(Value); }
		
        friend AltDecBase operator+(const signed int& lValue, const AltDecBase& rValue) { return rValue.AddByInt(lValue); }
        friend AltDecBase operator+(const signed long long& lValue, const AltDecBase& rValue) { return rValue.AddByInt64(lValue); }
        friend AltDecBase operator+(const unsigned int& lValue, const AltDecBase& rValue) { return rValue.AddByUInt(lValue); }
        friend AltDecBase operator+(const unsigned long long& lValue, const AltDecBase& rValue) { return rValue.AddByUInt64(lValue); }

        friend AltDecBase operator+(const AltDecBase& self, const signed char& Value) { return self.AddByInt8(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const signed short& Value) { return self.AddByInt16(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const unsigned char& Value) { return self.AddByUInt8(Value); }
        friend AltDecBase operator+(const AltDecBase& self, const unsigned short& Value) { return self.AddByUInt16(Value); }

        friend AltDecBase operator+(const signed char& lValue, const AltDecBase& rValue) { return rValue.AddByInt8(lValue); }
        friend AltDecBase operator+(const signed short& lValue, const AltDecBase& rValue) { return rValue.AddByInt16(lValue); }
        friend AltDecBase operator+(const unsigned char& lValue, const AltDecBase& rValue) { return rValue.AddByUInt8(lValue); }
        friend AltDecBase operator+(const unsigned short& lValue, const AltDecBase& rValue) { return rValue.AddByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltDecBase& operator+=(MediumDecV2Base& self, const signed int& Value) { return self.IntAddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const signed long long& Value) { return self.Int64AddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const unsigned long long& Value) { return self.UInt64AddOp(Value); }

        friend AltDecBase& operator+=(MediumDecBase& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend AltDecBase& operator+=(MediumDecBase& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

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
        auto& UIntSubOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntSubOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto SubByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase</returns>
        template<IntegerType IntType= signed int>
        auto SubByIntV1(const IntType& rValue)
		{
			//Add Code here
		}

public:

        constexpr auto UIntSubOpV1 = UIntSubOpV1<unsigned int>;
        constexpr auto IntSubOpV1 = IntSubOpV1<signed int>;
        constexpr auto UnsignedIntSubOp = UIntSubOpV1<signed int>;
        constexpr auto UInt64SubOp = UIntSubOpV1<unsigned long long>;
        constexpr auto Int64SubOp = IntSubOpV1<signed long long>;
	
        constexpr auto SubByUInt = SubByUIntV1<unsigned int>;
        constexpr auto SubByInt = SubByIntV1<signed int>;
        constexpr auto UnsignedSubByInt = SubByUInt<signed int>;
        constexpr auto SubByUInt64 = SubByUInt<;
        constexpr auto SubByInt64 = BasicSubByInt64;
        constexpr auto UnsignedSubByInt64 = SubByUInt<signed long long>;
		
        constexpr auto SubByUInt8 = BasicSubByUInt8;
        constexpr auto SubByInt8 = BasicSubByInt8;
        constexpr auto SubByUInt16 = BasicSubByUInt16;
        constexpr auto SubByInt16 = BasicSubByInt16;

        /// <summary>
        /// Unsigned subtraction operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& UnsignedSubOp(const auto& rValue);

        /// <summary>
        /// Subtraction operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& SubOp(const auto& Value)
        {
            if(Value<0)
                return UnsignedAddOp(-Value);
            else
                return UnsignedSubOp(Value);
        }

		/// <summary>
        /// Unsigned subtraction operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto SubtractByUnsigned(const auto& rValue)
        {
            auto self = *this;
            return self.UnsignedSubOp(rValue);
        }

		/// <summary>
        /// Subtraction operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto SubtractBy(const auto& rValue)
        {
            auto self = *this;
            return self.SubOp(rValue);
        }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase operator-(const AltDecBase& self, const AltDecBase& Value) { return self.SubtractBy(Value); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase& operator-=(AltDecBase& self, const AltDecBase& Value) { return self.SubOp(Value); }
		
        /// <summary>
        /// Subtraction operation between AltDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase operator-(const AltDecBase& self, const signed int& Value) { return self.SubtractByInt(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const signed long long& Value) { return self.SubtractByInt64(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const unsigned long long& Value) { return self.SubtractByUInt64(Value); }
		
        friend AltDecBase operator-(const signed int& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const signed long long& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const unsigned int& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const unsigned long long& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }

        friend AltDecBase operator-(const AltDecBase& self, const signed char& Value) { return self.SubtractByInt8(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const signed short& Value) { return self.SubtractByInt16(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const unsigned char& Value) { return self.SubtractByUInt8(Value); }
        friend AltDecBase operator-(const AltDecBase& self, const unsigned short& Value) { return self.SubtractByUInt16(Value); }

        friend AltDecBase operator-(const signed char& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const signed short& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const unsigned char& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }
        friend AltDecBase operator-(const unsigned short& lValue, const AltDecBase& rValue) { return ((AltDecBase)lValue).SubtractBy(rValue); }


        /// <summary>
        /// -= operation between AltDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend AltDecBase& operator-=(AltDecBase& self, const signed int& Value) { return self.IntSubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const signed long long& Value) { return self.Int64SubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const unsigned long long& Value) { return self.UInt64SubOp(Value); }

        friend AltDecBase& operator-=(AltDecBase& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend AltDecBase& operator-=(AltDecBase& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

    #pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
    //Defined inside full version of class object
	#pragma endregion Modulus Operations

    #pragma region Floating Operator Overrides
    
        friend AltDecBase operator+(const AltDecBase& self, const float& Value) { return self + (AltDecBase)Value; }
        friend AltDecBase operator-(const AltDecBase& self, const float& Value) { return self - (AltDecBase)Value; }
        friend AltDecBase operator*(const AltDecBase& self, const float& Value) { return self * (AltDecBase)Value; }
        friend AltDecBase operator/(const AltDecBase& self, const float& Value) { return self / (AltDecBase)Value; }

        friend AltDecBase operator+(const float& Value, const AltDecBase& self) { return (AltDecBase)Value + self; }
        friend AltDecBase operator-(const float& Value, const AltDecBase& self) { return (AltDecBase)Value - self; }
        friend AltDecBase operator*(const float& Value, const AltDecBase& self) { return (AltDecBase)Value * self; }
        friend AltDecBase operator/(const float& Value, const AltDecBase& self) { return (AltDecBase)Value / self; }

        friend AltDecBase operator+(const AltDecBase& self, const double& Value) { return self + (AltDecBase)Value; }
        friend AltDecBase operator-(const AltDecBase& self, const double& Value) { return self - (AltDecBase)Value; }
        friend AltDecBase operator*(const AltDecBase& self, const double& Value) { return self * (AltDecBase)Value; }
        friend AltDecBase operator/(const AltDecBase& self, const double& Value) { return self / (AltDecBase)Value; }

        friend AltDecBase operator+(const AltDecBase& self, const ldouble& Value) { return self + (AltDecBase)Value; }
        friend AltDecBase operator-(const AltDecBase& self, const ldouble& Value) { return self - (AltDecBase)Value; }
        friend AltDecBase operator*(const AltDecBase& self, const ldouble& Value) { return self * (AltDecBase)Value; }
        friend AltDecBase operator/(const AltDecBase& self, const ldouble& Value) { return self / (AltDecBase)Value; }

        friend AltDecBase operator+(const ldouble& Value, const AltDecBase& self) { return (AltDecBase)Value + self; }
        friend AltDecBase operator-(const ldouble& Value, const AltDecBase& self) { return (AltDecBase)Value - self; }
        friend AltDecBase operator*(const ldouble& Value, const AltDecBase& self) { return (AltDecBase)Value * self; }
        friend AltDecBase operator/(const ldouble& Value, const AltDecBase& self) { return (AltDecBase)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators
	
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>AltDecBase</returns>
        AltDecBase operator- ()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++AltDecBase Operator
        /// </summary>
        /// <returns>AltDecBase &</returns>
        AltDecBase& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntValue;
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
            else
                ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++AltDecBase Operator
        /// </summary>
        /// <returns>AltDecBase &</returns>
        AltDecBase& operator --()
        {
            if (DecimalHalf == 0)
                --IntValue;
            else if (IntValue == 0)
                IntValue = NegativeRep;
            else
                --IntValue;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>AltDecBase</returns>
        AltDecBase operator ++(int)
        {
            AltDecBase tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>AltDecBase</returns>
        AltDecBase operator --(int)
        {
            AltDecBase tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>AltDecBase &</returns>
        AltDecBase& operator *()
        {
            return *this;
        }
		
    #pragma endregion Other Operators

	#pragma region Math Etc Functions

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        auto SqrtOf(const int& precision=7)
        {
    #if !defined(AltNum_EnableIRep)
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
    #endif
            auto value = ConvertAsNormTypeV2();
            if (value.DecimalHalf.Value == 0&&value.DecimalHalf.Flags==0)
            {
                bool AutoSetValue = true;
                switch (IntValue.Value)
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
                case 1600: value.IntValue = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                {
    #if defined(AltNum_EnableIRep)
                    if(IsNegative())
                        DecimalHalf.Flags = 3;
    #endif
                    return value;//Techically both positive and negative numbers of same equal the result
                }
            }

            auto number = this;
            auto start = 0, end = number;
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

	#if defined(AltNum_EnableIRep)
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// without flipping of negative status and other checks
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto PartialUIntPowOp(const ValueType& expValue)
        {
            if (DecimalHalf.Value == 0 && IntValue.Value == 10)
            {
                IntValue.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf.Value = 0; ExtraRep = 0;
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                auto self = AbsOf();
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
        /// without flipping of negative status and other checks
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto PartialIntPowOfOp(const ValueType& expValue)
        {
            if (expValue < 0)//Negative Pow
            {
                ValueType exp = expValue * -1;
				//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
				auto self = AbsOf();
				IntValue = 1; DecimalHalf = 0;// Initialize result
				while (expValue > 0)
				{
					// If expValue is odd, multiply self with result
					if (exp & 1 == 1)
						*this /= self;
					// n must be even now
					expValue = expValue >> 1; // y = y/2
					self = self / self; // Change x to x^-1
				}
                return *this;
            }
            else if (DecimalHalf.Value == 0 && IntValue.Value == 10)
            {
                IntValue.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf.Value = 0; ExtraRep = 0;
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                auto self = AbsOf();
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
    #endif

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// without checking for specific representation type
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicUIntPowOpV1(const ValueType& expValue)
        {
            auto convertedVal = ConvertAsNormTypeV2();
            if (convertedVal.DecimalHalf == 0 && convertedVal.IntValue.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntValue.IsPositive = 1;
                IntValue.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf = 0; ExtraRep = 0;
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
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
                if(IsNegative)
                    IntValue.IsPositive = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// without checking for specific representation type
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicIntPowOfOpV1(const ValueType& expValue)
        {
            auto convertedVal = ConvertAsNormTypeV2();
            if (expValue < 0)//Negative Pow
            {
                auto convertedVal = ConvertAsNormTypeV2();
                ValueType exp = expValue * -1;
                if (convertedVal.DecimalHalf.Value == 0 && convertedVal.IntValue == 10 && expValue >= -9)
                {
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&exp&1==1)
                        IsPositive = 1;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                    auto self = AbsOf();
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (exp & 1 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    if(IsNegative)
                        IntValue.IsPositive = 0;
                }
                return *this;
            }
            else if (convertedVal.DecimalHalf.Value == 0 && convertedVal.IntValue.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntValue.IsPositive = 1;
                IntValue.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf = 0; ExtraRep = 0;
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
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
                if(IsNegative)
                    IntValue.IsPositive = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicUIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.BasicUIntPowOpV1();
		}
		
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.BasicIntPowOpV1();
		}

public:

        constexpr auto BasicUIntPowOfOp = BasicUIntPowOfOpV1<unsigned int>;
        constexpr auto BasicIntPowOfOp = BasicIntPowOfOpV1<signed int>;
        constexpr auto BasicUInt64PowOfOp = BasicUIntPowOfOpV1<unsigned long long>;
        constexpr auto BasicInt64PowOfOp = BasicIntPowOpOfV1<signed long long>;
        
        constexpr auto BasicUIntPowOf = BasicUIntPowOfV1<unsigned int>;
        constexpr auto BasicIntPowOf = BasicIntPowOfV1<signed int>;
        constexpr auto BasicUInt64PowOf = BasicUIntPowOfV1<unsigned long long>;
        constexpr auto BasicInt64PowOf = BasicIntPowOfV1<signed long long>;

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        constexpr auto NthRootOf = MediumDecBase::NthRootOf;

        /// <summary>
        /// Multiply by Pi exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto MultiplyByPiPower(const ValueType& exp)
        {
        }

        /// <summary>
        /// Multiply by E exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto MultiplyByEPower(const ValueType& exp)
        {
        }

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOpV1(const ValueType& exp)
        {
			if (expValue == 1)
				return *this;//Return self
			else if (expValue == 0)
				SetAsOne(); return *this;
		#if defined(AltNum_EnablePiRep)
			else if(DecimalHalf.Flags==1)
			{
				BasicUIntPowOp(expValue);
				MultiplyByPiPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableERep)
			else if(DecimalHalf.Flags==2)
			{
				if (expValue == 1)
					return *this;//Return self
				else if (expValue == 0)
					SetAsOne(); return *this;
				BasicUIntPowOp(expValue);
				MultiplyByEPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableIRep)
            else if(DecimalHalf.Flags==3)
            {
				if (expValue == 1)
					return *this;//Return self
				else if (expValue == 0)
					SetAsOne(); return *this;
                //Add code here
                return *this;
            }
        #endif
        #if defined(AltNum_EnableInfinityRep)
            else if(DecimalHalf.Value==InfinityRep)
            {
            }
        #endif
        #if defined(AltNum_EnableApproaching)
            else if (DecimalHalf == ApproachingBottomRep)
            {
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
            }
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)
            else if(DecimalHalf.Value==UndefinedInRangeRep)
            {
            }
        #endif
        #if defined(AltNum_EnableWithinMinMaxRange)
            else if(ExtraRep==WithinMinMaxRangeRep)
            {
            }
        #endif
            else
                BasicUIntPowOpV1(expValue); 
			return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfOpV1(const ValueType& expValue)
        {
        #if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf.Value==InfinityRep)
            {
            }
        #endif
        #if defined(AltNum_EnableApproaching)
            if (DecimalHalf == ApproachingBottomRep)
            {
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
            }
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)
            else if(DecimalHalf.Value==UndefinedInRangeRep)
            {
            }
        #endif
        #if defined(AltNum_EnableWithinMinMaxRange)
            else if(ExtraRep==WithinMinMaxRangeRep)
            {
            }
        #endif
            else
                BasicIntPowOpV1(expValue); 
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
        constexpr auto UInt64PowOfOp = UIntPowOfOpV1<unsigned long long>;
        constexpr auto Int64PowOfOp = IntPowOpOfV1<signed long long>;
        
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
        constexpr auto UInt64PowOf = UIntPowOfV1<unsigned long long>;
        constexpr auto Int64PowOf = IntPowOfV1<signed long long>;

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <returns>MediumDecBase</returns>
        constexpr auto ExpOf = MediumDecV2Base::ExpOf;

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        constexpr auto Exp = MediumDecV2Base::Exp;

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        constexpr auto NthRootOf = MediumDecV2Base::NthRootOf;

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        constexpr auto NthRoot = MediumDecV2Base::NthRoot;

		/// <summary>
		/// Natural log (Equivalent to Log_E(value))
		/// </summary>
		/// <returns>BlazesRusCode::MediumDecBase</returns>
        constexpr auto NaturalLogOf = MediumDecV2Base::NaturalLogOf;
	
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        constexpr auto Ln = MediumDecV2Base::Ln;
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Log10Of = MediumDecV2Base::Log10Of;
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Log10 = MediumDecV2Base::Log10;

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Log10OfInt = MediumDecV2Base::Log10OfInt;
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto LogOf = MediumDecV2Base::LogOf;
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Log = MediumDecV2Base::Log;

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto LogOfInt = MediumDecV2Base::LogOfInt;

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto LogOfV2 = MediumDecV2Base::LogOfV2;

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions
public:
       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Sin = MediumDecV2Base::Sin;

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Cos = MediumDecV2Base::Cos;

        /// <summary>
        /// Get Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto Tan = MediumDecV2Base::Tan;

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto ATan = MediumDecV2Base::ATan;

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto ArcTan2 = MediumDecV2Base::ArcTan2;

        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto SinFromAngle = MediumDecV2Base::SinFromAngle;

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns></returns>
        constexpr auto CosFromAngle = MediumDecV2Base::CosFromAngle;

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto TanFromAngle = MediumDecV2Base::TanFromAngle;

    #pragma endregion Trigonomic Functions

    #pragma endregion Trigonomic Functions
    }
    #pragma region String Function Source

    std::string AltDecBase::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::Infinity:
            return IsNegative()?"-∞":"∞";
            break;
	    #if defined(AltNum_EnableApproaching)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom);
			#else
            return (std::string)IntValue + ".0..01";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop);
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            return ConvertToBasicString(repType);
			break;
            #endif
        #endif
	#endif

    #if defined(AltNum_EnableFractionals)
        case RepType::NumByDiv:
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value);
            break;
    #endif

	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToStringOp()+"π";
            break;

        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep.Value))*Pi Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"π";
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
        case RepType::ENumByDiv://  (Value/(ExtraRep.Value))*e Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif

	#endif

	#if defined(AltNum_EnableIRep)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;

        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep.Value))*i Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"i";
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
			return ConvertToBasicString(RepType::ApproachingTop)+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::ImaginaryInfinity:
            return IsNegative()?"-∞i":"∞i";
            break;
	#endif
	
	#if defined(AltNum_EnableApproachingI)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"i";
			#else
            return (std::string)IntValue + ".0..01i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"i";
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

    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf.Value)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf.Value)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"π";
            break;
		#endif
		#if defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf.Value)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"e";
            break;
		#endif
		#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf.Value)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep.Value)+"i";
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
		    return "WithinMinMaxRange of "+VariableConversionFunctions::UnsignedIntToStringConversion((int)IntValue)+" to "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf);
            break;
        #endif

	#endif
    #if defined(AltNum_EnableNil)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			return ConvertToBasicString(repType);
            break;
        }
    }

    #pragma endregion String Function Source
}
