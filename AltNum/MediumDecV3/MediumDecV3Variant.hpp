// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <compare>//used for C++20 feature of spaceship operator

#include "..\AlternativeInt\PartialInt.hpp"
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
  class MediumDecV3Variant;
	
	

  /**
   * Variants (via Policy presets):
   *   - MediumDecV3VariantV3 : 0 .. 4,294,967,295.999999999 — tuned defaults for unsigned range
   *   - MediumDecV3  : ±2,147,483,647.999999999 — tuned defaults for signed range
   *   - MediumDecV4  : DecV3 + special reps (Pi/e/i, Infinity, approaching states, NaN, etc.)
   *   - MediumUDecV4 : UDecV3 + same special reps
   *   - AltDecV2     : DecV4 + fractional + optional indeterminate; other ExtraRep via Policy
   *
   * - Variants are aliases to MediumDecV3Variant<TPolicy> with curated defaults.
   * - Any variant can enable/disable additional features via Policy without separate builds.
   * - MediumDecV3Variant and these presets are safe derivation targets; base internals are not.
	 *
	 * Conexpr settings partially based on initial policy but final settings based on combination of flags from policy
  */
  template<class VariantName, typename Policy:FixedDecDefaultPolicy>
  class MediumDecV3Variant
	{
	protected:
	  using RepTypeUnderlayer as unsigned int;//Adjust based on policy later
  //Constructor type enum

  //Doesn't derive directly from Policy instead checks policy and creates policy based constexpr based on flags
  #pragma region Policy_extractor

  #pragma endregion Policy_extractor
  public:
	//RepTypeV2 (templatable version of RepType for MediumDecV3Variant based on policy settings
	
	
	
  #pragma region class_constructors


    VariantName& operator=(const VariantName& rhs)
    {
      // Check for self-assignment
      if (this == &rhs)    // Same object?
        return *this;    // Yes, so skip assignment, and just return *this.
      if(
        IntHalf = rhs.IntHalf();
        DecimalHalf = rhs.DecimalHalf();
      } else {//Bitwise based storage
        //MediumDecV2 and lower all store information in just the raw object
        raw = rhs.raw;
        //If ExtraRep enabled
      }
      return *this;
    }

    VariantName& operator=(const signed int& rhs)
    {
      IntHalf() = rhs; DecimalHalf() = 0;
      return *this;
    }


  #pragma endregion class_constructors

  #pragma region Negative_Status

    /// <summary>
    /// Negative Unary Operator(Flips negative status)
    /// </summary>
    /// <param name="self">The self.</param>
    /// <returns>VariantName</returns>
    VariantName operator-() const
    { VariantName self = *this; self.SwapNegativeStatus(); return self; }

  #pragma endregion Negative_Status

  #pragma region Check_if_value
  #pragma endregion Check_if_value

  #pragma region RangeLimits
  #pragma endregion RangeLimits

  #pragma region ValueSetters
protected://Work around for not allowing to use incomplete class statics during forming of class
    static const unsigned int LN10Div_DecSection = 434294482;
    static const unsigned int TwiceLN10Div_DecSection = 868588964;

public:

    /// <summary>
    /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
    /// (Stored as 3.141592654)
    /// </summary>
    void  SetValueToPiNum()
		{//Closest RestrictedFloat value to match 10th+ digits of pi (0, 39 580 354)
			if constexpr (Policy::DisableBitwiseMaskMode) {
				IntHalf = 3; DecimalHalf = 141592654;
			}
			else
			{
			
			}
		}

    //100,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToHundredMilPiNum()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
				IntHalf = 314159265; DecimalHalf = 358979324;
			}
			else
			{
			
			}
		}

    //10,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToTenMilPiNum()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
	      IntHalf = 31415926; DecimalHalf = 535897932;
			}
			else
			{
			
			}
		}

    //1,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToOneMilPiNum()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 3141592; DecimalHalf = 653589793;
			}
			else
			{
			
			}
		}

    //10xPi(Rounded to 9th decimal digit)
    void  SetValueToTenPiNum(){
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 31; DecimalHalf = 415926536;
			}
			else
			{
			
			}
		}


    /// <summary>
    /// Euler's number rounded to 9th digit(2.718281828)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    void  SetValueToENum(){
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 2; DecimalHalf = 718281828;
			}
			else
			{
			
			}
		}

    //Sets value to value at 0.5
    void  SetValueToPoint5(){
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 500000000;
			}
			else
			{
			
			}
		}

    void  SetValueToJustAboveZero()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 1;
			}
			else
			{
			
			}
		}
    /// <summary>
    /// Sets the value at .000001000
    /// </summary>
    void  SetValueToOneMillionth()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 1000;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// Sets the value at "0.005"
    /// </summary>
    /// <returns>MediumDecV3Variant</returns>
    void  SetValueToFiveThousandth()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 5000000;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// Sets the value at "0.000005"
    /// </summary>
    void  SetValueToFiveMillionth()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 5000;
			}
			else
			{
			
			}
		}

    //0e-7
    void  SetValueToTenMillionth()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 100;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// Sets the value to .000000010
    /// </summary>
    void  SetValueToOneHundredMillionth()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 10;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// 2.3025850929940456840179914546844
    /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
    /// </summary>
    void  SetValueToLN10()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
	      IntHalf = 2; DecimalHalf = 302585093;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToLN10Div()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
      	IntHalf = 0; DecimalHalf = 434294482;
			}
			else
			{
			
			}
		}

    /// <summary>
    /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToTwiceLN10Div()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
        IntHalf = 0; DecimalHalf = 868588964;
			}
			else
			{
			
			}
		}

    void SetValueToPointOne()
		{
			if constexpr (Policy::DisableBitwiseMaskMode) {
        IntHalf = 0; DecimalHalf = 100000000;
			}
			else
			{
			
			}
		}

  #pragma endregion ValueSetters

  #pragma region ValueDefines

  #pragma endregion ValueDefines

  #pragma region String Commands

  #pragma endregion String Commands

  #pragma region ConvertFromOtherTypes

  #pragma endregion ConvertFromOtherTypes

  #pragma region ConvertToOtherTypes

  #pragma endregion ConvertToOtherTypes

  #pragma region Pi Conversion

  #pragma endregion Pi Conversion

  #pragma region E Conversion

  #pragma endregion E Conversion

  #pragma region region Imaginary Conversion

  #pragma endregion region Imaginary Conversion

  #pragma region Other RepType Conversion

  #pragma endregion Other RepType Conversion

  #pragma region Comparison Operators

   /*
		//Converts Representation down to basic PiNum,ENum,INum, and NormalType representations 
		void ConvertDownToMediumDecV2Equiv()
		{
			RepTypeV2 repType = GetRepType();
			switch(repType)
			{
		#if defined(AltNum_EnableMixedFractional)
			#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::MixedPi:
			#endif
			#if defined(AltNum_EnableERep)
				case RepTypeEnum::MixedE:
			#endif
			#if defined(AltNum_EnableIRep)
				case RepTypeEnum::MixedI:
			#endif
				case RepTypeEnum::MixedFrac:
					ConvertFromMixedFracToMediumDecV2Equiv(); break;
		#elif defined(AltNum_EnablePowerOfRepresentation)
			#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::PiPower:
					ConvertPiPowerToPiRep(); break;
			#endif
			#if defined(AltNum_EnableERep)
				case RepTypeEnum::EPower:
					ConvertEPowerToERep(); break;
			#endif
				case RepTypeEnum::ToPowerOf:
					ConvertToNormType(repType); break;
		#endif
		#if defined(AltNum_EnableFractionals)
			#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::PiNumByDiv:
			#endif
			#if defined(AltNum_EnableERep)
				case RepTypeEnum::ENumByDiv:
			#endif
			#if defined(AltNum_EnableIRep)
				case RepTypeEnum::INumByDiv:
			#endif
				case RepTypeEnum::NumByDiv:
					BasicIntDivOp(ExtraRep.Value);
					ResetDivisor();
		#endif
				default:
					ConvertToNormType(repType);
			}
			ConvertToNormTypeV2();
		}*/

		std::strong_ordering operator<=>(const VariantName& that) const
		{
			return CompareWith(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return CompareWithInt(that);
		}

		bool operator==(const int& that) const
		{
      if constexpr (Base::DisableBitwiseMaskMode) {
  			if (IntHalf!=that)
  				return false;
  			if (DecimalHalf!=0)
  				return false;
  			return true;
      } else {
      }
		}

		bool operator==(const MediumDec& that) const
		{
      if constexpr (Base::DisableBitwiseMaskMode) {
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
    			VariantName LValue = this;
    			LValue.ConvertToNormTypeV2();
    			MediumDec RValue = that;
    			RValue.ConvertToNormTypeV2();
    		#else
    			VariantName LValue = this;
    			MediumDec RValue = that;
    			if(DecimalHalf.Flags!=0)
    				return false;
    		#endif
    			if (LValue.IntHalf!=RValue.IntHalf)
    				return false;
    			if (LValue.DecimalHalf!=RValue.IntHalf)
    				return false;
      } else {
      }
    }

		bool operator==(const MediumDecV2& that) const
		{
      if constexpr (Base::DisableBitwiseMaskMode) {
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
			VariantName LValue = this;
			LValue.ConvertToNormTypeV2();
			MediumDecV2 RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			VariantName LValue = this;
			MediumDecV2 RValue = that;
			if(DecimalHalf.Flags==RValue.DecimalHalf.Flags)
				LValue.ConvertDownToMediumDecV2Equiv();
			else if((DecimalHalf.Flags==3 && RValue.DecimalHalf.Flags!=3)||(RValue.DecimalHalf.Flags==3 && LValue.DecimalHalf.Flags!=3))
				throw "Can't compare imaginary number with real number";
			else if(DecimalHalf.Flags!=0)
				return false;
		#endif
			if (LValue.IntHalf!=RValue.IntHalf)
				return false;
			if (LValue.DecimalHalf!=RValue.IntHalf)
				return false;
      } else {
      }
		}

		bool operator==(const VariantName& that) const
		{
      if constexpr (Base::DisableBitwiseMaskMode) {
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
    			VariantName LValue = this;
    			LValue.ConvertToNormTypeV2();
    			VariantName RValue = that;
    			RValue.ConvertToNormTypeV2();
    		#else
    			VariantName LValue = this;
    			VariantName RValue = that;
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
    			if (LValue.IntHalf!=RValue.IntHalf)
    				return false;
    			if (LValue.DecimalHalf!=RValue.IntHalf)
    				return false;
      } else {
      }
    }

  #pragma endregion Comparison Operators

  #pragma region NormalRep Integer Division operations

  #pragma endregion NormalRep Integer Division operations

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

  #pragma region NormalRep AltNum division operations

  #pragma endregion NormalRep AltNum division operations

  #pragma region NormalRep AltNum Multiplication Operations

  #pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Other division operations

	#pragma endregion Other division operations

  #pragma region Other multiplication operations

  #pragma endregion Other multiplication operations

  #pragma region Other addition operations

  #pragma endregion Other addition operations

  #pragma region Other Subtraction Operations
  #pragma endregion Other Subtraction Operations

  #pragma region Modulus Operations

  #pragma endregion Modulus Operations

  #pragma region Bitwise Operations

  #pragma endregion Bitwise Operations

  #pragma region Floating Operator Overrides
  #pragma endregion Floating Operator Overrides

  #pragma region Other Operators

    //Reverses DecimalHalf over DecimalOverflow limit(moving to another section later)
    inline void FlipDecimalHalf(){
      if constexpr (DisableBitwiseMaskMode) {
        DecimalHalf = DecimalOverflow - DecimalHalf;
      else
        DecimalHalf() = DecimalOverflow - DecimalHalf();
    }


    /// <summary>
    /// ++MediumDecV3Variant Operator
    /// </summary>
    /// <returns>MediumDecV3Variant &</returns>
    MediumDecV3Variant& operator ++()
    {
      //Convert to normal type if needed before operation decrease
      if constexpr (DisableBitwiseMaskMode) {
        if (constexpr (UnsignedMode))
          ++IntHalf;
        else {
          if (DecimalHalf == 0)
            ++IntHalf;
          else if (IntHalf == IntHalfNegZero) {
            IntHalf = IntHalfZero;
            FlipDecimalHalf();
          }
          else
            ++IntHalf;
      } else {
        if (constexpr (UnsignedMode))
          ++IntHalf();
        else {
          if (DecimalHalf == 0)
            ++IntHalf();
          else if (IntHalf == IntHalfNegZero) {
            FlipSign();
            FlipDecimalHalf();
          }
          else
            ++IntHalf();
      }
      return *this;
    }

    /// <summary>
    /// ++MediumDecV3Variant Operator
    /// </summary>
    /// <returns>MediumDecV3Variant &</returns>
    VariantName& operator --()
    {
      //Convert to normal type if needed before operation decrease
      if constexpr (Policy::DisableBitwiseMaskMode) {
        if (constexpr (UnsignedMode))
          --IntHalf;
        else {
          if (DecimalHalf == 0)
            --IntHalf;
          else if (IntHalf == IntHalfZero)
            IntHalf = IntHalfNegZero;
            FlipDecimalHalf();
          else
            --IntHalf;
        }
      } else {
        if (constexpr (UnsignedMode))
          --IntHalf;
        else {
          if(DecimalHalf==0)
            --IntHalf();
          else if (IntHalf()==0){
            FlipSign();
            FlipDecimalHalf();
          } else
            --IntHalf;
        }
      }
      return *this;
    }

    /// <summary>
    /// MediumDecV3Variant Variant++ Operator
    /// </summary>
    /// <returns>MediumDecV3Variant</returns>
    VariantName operator ++(int)
    {
      VariantName tmp(*this);
      ++* this;
      return tmp;
    }

    /// <summary>
    /// MediumDecV3Variant Variant-- Operator
    /// </summary>
    /// <returns>MediumDecV3Variant</returns>
    VariantName operator --(int)
    {
      VariantName tmp(*this);
      --* this;
      return tmp;
    }

    /// <summary>
    /// MediumDecV3Variant Variant* Operator
    /// </summary>
    /// <returns>MediumDecV3Variant &</returns>
    VariantName& operator *()
    {
      return *this;
    }

  #pragma endregion Other Operators

  #pragma region Truncation Functions
  #pragma endregion Truncation Functions

  #pragma region Pow and Sqrt Functions
  #pragma endregion Pow and Sqrt Functions

  #pragma region Log Functions
  #pragma endregion Log Functions

  #pragma region Trigonomic Functions
  #pragma endregion Trigonomic Functions
  };

  #pragma region String Function Source

  #pragma endregion String Function Source
}