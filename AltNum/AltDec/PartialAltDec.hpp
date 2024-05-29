// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\MediumDecV2\MediumDecV2Base.hpp"
#include "..\MediumDecV2\MediumDecV2.hpp"
#if !defined(AltNum_EnableNegativePowerRep)
	#include "..\AlternativeInt\FlaggedInt.hpp"
#endif
#include "AltDecPreprocessors.h"

namespace BlazesRusCode
{
    class PartialAltDec;

    //Reduced version of AltDec result for modulus result and other stuff
    class DLL_API PartialAltDec : public MediumDecBaseV2
    {
public:
#pragma region DigitStorage

        /// <summary>
        /// Multiplied by Pi, e, or i if DecimalHalf.Flags!=0
        /// If ExtraRep is zero and DecimalHalf.Value<999999999, then AltDecBase represents +- 2147483647.999999999
	#if defined(AltNum_EnableFractionals)
		/// If ExtraRep is greator than zero, then AltDecBase represents +- 2147483647.999999999
	#endif
	#if defined(AltNum_EnableNegativePowerRep)
		/// If ExtraRep is not zero, than representation number to the power of ExtraRep
	#elif defined(AltNum_EnablePowerOfRepresentation)
		/// If ExtraRep.IsAltRep==1, than is representation number to the power of ExtraRep	
	#elif defined(AltNum_EnableMixedFractional)
		/// If ExtraRep.IsAltRep==1, than is representation number is a mixed fraction	
	#endif
        /// </summary>
        DivisorType ExtraRep;

		static const unsigned int InitialExtraRep = 1;

		void ResetDivisor(){ ExtraRep = InitialExtraRep; }

#pragma endregion DigitStorage

#pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialAltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        PartialAltDec(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero, const FlaggedInt& extraVal = InitialExtraRep)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialAltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        PartialAltDec(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero, const FlaggedInt& extraVal = InitialExtraRep)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        PartialAltDec& operator=(const PartialAltDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = rhs.ExtraRep;
            return *this;
        }

        PartialAltDec& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            ResetDivisor();
            return *this;
        }

#pragma endregion class_constructors

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const PartialAltDec& Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        } const

		//Set value as exactly zero
        void SetAsZero()
        {
            IntHalf = 0;
            DecimalHalf = 0; ResetDivisor();
        }

		//Set value as exactly one
        void SetAsOne()
        {
            IntHalf = 1;
            DecimalHalf = 0; ResetDivisor();
        }
		
		//Set as +-1 while keeping current sign
        void SetAsOneVal()
        {
            IntHalf.Value = 1;
            DecimalHalf = 0; ResetDivisor();
        }

    };

}