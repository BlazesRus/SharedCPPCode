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


#include "..\MirroredInt.hpp"
#include "..\PartialInt.hpp"
#include "..\RepType.h"

//"Not used for this variant" comment used as placeholder
// between unused regions to help with code compare between variants and keep structure similar
//Use  template<MediumDecVariant VariantType=MediumDecBase>
//to template functions for reuse with VariantTypes

namespace BlazesRusCode
{
    class MediumDecBase;
	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;
	
	// Forward declare VTable instances.
	//extern AltNum_VTable MediumDecBase_vtable;

/*---Accuracy Tests(with MediumDecBase based settings):
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
    class DLL_API MediumDecBase
    {
protected:
#if defined(AltNum_UseBuiltinVirtualTable)
		template <typename T>
		void** GetVTable(T* obj) {
			return *((void***)obj);
		}
#endif

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
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDecBase& operator=(const MediumDecBase& rhs)
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