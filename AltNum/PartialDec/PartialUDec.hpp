// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV2Preprocessors.h"
#include "..\MediumDec\MediumUDec.hpp"
#if defined(AltNum_UseBuiltinVirtualTable)
    #include "..\VirtualTableBase.hpp"
#endif


using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;
#if !defined(AltNum_UseRepTypeAsClass)
using RepTypeEnum = BlazesRusCode::RepType;
#endif

namespace BlazesRusCode
{
	/// <summary>
	/// Alternative Non-Integer number representation with focus on accuracy and speed within certain range
	/// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
	/// plus fractional representation
	/// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
	class DLL_API PartialUDec : public MediumUDec
	{
	public:
    //Divisor of Fractional
    unsigned int ExtraRep;
		
	#pragma region class_constructors
	
		/// <summary>
		/// Initializes a new instance of the <see cref="PartialUDec"/> class.
		/// </summary>
		/// <param name="intVal">The whole number based half of the representation</param>
		/// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
		PartialUDec(const unsigned int& intVal=0, const PartialInt& decVal = PartialInt::Zero, const unsigned int& extraVal = 1)
		{
			IntHalf = intVal;
			DecimalHalf = decVal;
			ExtraRep = extraVal;
		}
		
		PartialUDec(const PartialUDec&) = default;
		
		PartialUDec& operator=(const int& rhs)
		{
			IntHalf = rhs;
			DecimalHalf = 0;
			ExtraRep = InitialExtraRep;
			return *this;
		} const

		PartialUDec& operator=(const MediumUDec& rhs)
		{
			// Check for self-assignment
			if (this == &rhs)  // Same object?
			return *this;  // Yes, so skip assignment, and just return *this.
			IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
			ExtraRep = rhs.ExtraRep;
			return *this;
		} const

		PartialUDec& operator=(const MixedUDec& rhs)
		{
			// Check for self-assignment
			if (this == &rhs)  // Same object?
			return *this;  // Yes, so skip assignment, and just return *this.
			IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
			ExtraRep = rhs.ExtraRep;
			return *this;
		} const
		
	#pragma endregion class_constructors
	
		/// <summary>
		/// Sets the value.
		/// </summary>
		/// <param name="Value">The value.</param>
		void SetValue(const PartialUDec& rValue)
		{
				IntHalf = rValue.IntHalf;
				DecimalHalf.SetValueV2(rValue.DecimalHalf);
		}
	
    // For the IntHalf type (could be uint64_t or __int128 depending on your build)
		static unsigned int gcd_int(unsigned int a, unsigned int b) noexcept {
				// Euclidean algorithm
				while (b != 0) {
						unsigned int t = b;
						b = a % b;
						a = t;
				}
				return a;
		}
		
    // Reduce numerator/denominator by their greatest common divisor
    void normalizeSafe() noexcept {
        auto g = gcd_int(this->value(), ExtraRep);
        if (g > 1) {
            this->setValue(this->value() / g);
            ExtraRep /= g;
        }
    }
	
		//void scaleUp(unsigned int m) {
		//		num *= m;
		//		// no need to touch divisor here
		//}

		void scaleDown(unsigned int d) {
				// Try gcd reduction before multiply
				auto g = gcd(*this, d);
				if (g > 1) { *this /= g; d /= g; }
				divisor *= d;
		}
	
	};
}