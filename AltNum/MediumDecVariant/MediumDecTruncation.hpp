// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"

#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#include "..\Concepts\IntegerConcept.hpp"

namespace BlazesRusCode
{
	class DLL_API MediumDecTruncation
	{
  #pragma region Truncation Functions

    /// <summary>
    /// Forces Number into non-negative
    /// </summary>
    void ApplyAbs(){ IntHalf.ApplyAbs(); }

protected://Using templating as static function instead as fix for *this pointer not updating when used in other classes

    ///<summary>
    /// Returns a copy of the number converted into non-negative version
    ///</summary>
    template<typename VariantType>
    static VariantType Abs(VariantType tValue){
      tValue.ApplyAbs(); return tValue;
    }


    /// <summary>
    /// Returns floored value with all fractional digits after specified precision cut off.
    /// </summary>
    /// <param name="precision">precision level of digits to cut off</param>
    template<typename VariantType>
    void Floor(VariantType tValue, const int& precision = 0){
			switch (precision)
			{
			case 8: tValue.DecimalHalf.Value /= 10; tValue.DecimalHalf.Value *= 10; break;
			case 7: tValue.DecimalHalf.Value /= 100; tValue.DecimalHalf.Value *= 100; break;
			case 6: tValue.DecimalHalf.Value /= 1000; tValue.DecimalHalf.Value *= 1000; break;
			case 5: tValue.DecimalHalf.Value /= 10000; tValue.DecimalHalf.Value *= 10000; break;
			case 4: tValue.DecimalHalf.Value /= 100000; tValue.DecimalHalf.Value *= 100000; break;
			case 3: tValue.DecimalHalf.Value /= 1000000; tValue.DecimalHalf.Value *= 1000000; break;
			case 2: tValue.DecimalHalf.Value /= 10000000; tValue.DecimalHalf.Value *= 10000000; break;
			case 1: tValue.DecimalHalf.Value /= 100000000; tValue.DecimalHalf.Value *= 100000000; break;
			case 0:
				tValue.DecimalHalf = 0;
				break;
			default:
				break;
			}
			if (tValue.IntHalf == MirroredInt::NegativeZero && tValue.DecimalHalf == 0)
				tValue.IntHalf = 0;
			return tValue;
	}

    template<typename VariantType>
    static VariantType Ceil(const VariantType& tValue)
    {
      if(tValue.IntHalf==MirroredInt::NegativeZero)
        return VariantType::One;
      else if (tValue.DecimalHalf != 0)
        return VariantType(tValue.IntHalf+1);
      else
        return tValue;
    }

    /// <summary>
    /// Returns floored value with all fractional digits after specified precision cut off.
    /// </summary>
    /// <param name="tValue">The target value to apply on.</param>
    /// <param name="precision">The precision.</param>
    template<MediumDecVariant VariantType = MediumDec>
    static VariantType FloorV1(const VariantType& tValue, const int& precision = 0)
    {
      unsigned int decimalRes = tValue.DecimalHalf.Value;
      switch (precision)
      {
      case 8: decimalRes /= 10; decimalRes *= 10; break;
      case 7: decimalRes /= 100; decimalRes *= 100; break;
      case 6: decimalRes /= 1000; decimalRes *= 1000; break;
      case 5: decimalRes /= 10000; decimalRes *= 10000; break;
      case 4: decimalRes /= 100000; decimalRes *= 100000; break;
      case 3: decimalRes /= 1000000; decimalRes *= 1000000; break;
      case 2: decimalRes /= 10000000; decimalRes *= 10000000; break;
      case 1: decimalRes /= 100000000; decimalRes *= 100000000; break;
      default: decimalRes = 0; break;
      }
      if(decimalRes==0&&tValue.IntHalf==MirroredInt::NegativeZero)
        return VariantType();
      else
        return VariantType(tValue.IntHalf, PartialInt(decimalRes,tValue.DecimalHalf.Flags));
    }

    template<MediumDecVariant VariantType = MediumDec>
    const VariantType TruncOfV1() const
    {
      return VariantType(IntHalf == NegativeRep?0:IntHalf, 0);
    }


  #pragma endregion Truncation Functions
	};
}