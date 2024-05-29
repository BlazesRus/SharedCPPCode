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
#include "..\Concepts\IntegerConcept.hpp"
//#include "..\Concepts\MediumDecVariantConcept.hpp"


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

    }
}