// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#if defined(AltNum_MinimizeRepTypeEnum)
	#define RepTypeUnderlayer unsigned short
#else
	#define RepTypeUnderlayer unsigned int
#endif

namespace BlazesRusCode
{
	/// <summary>
	/// Enum representing value type stored
	/// </summary>
#if defined(AltNum_UseJustEnumForRepType)
	enum class RepType: RepTypeUnderlayer
#else
	enum class RepTypeEnum: RepTypeUnderlayer
#endif
	{
		NormalType = 0,
#if defined(AltNum_EnableFractionals)
		NumByDiv = 8,
#endif
#if defined(AltNum_EnablePowerOfRepresentation)
		ToPowerOf = 16,
#endif
#if defined(AltNum_EnablePiRep)
		PiNum = 1,
	#if defined(AltNum_EnablePowerOfRepresentation)
		PiPower = 17,
	#endif
	#if defined(AltNum_EnableFractionals)
		PiNumByDiv = 9,//  (Value/(ExtraRep.Value))*Pi Representation
	#endif
	#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7,1)
		//equal to IntValue.0..01 Pi
		ApproachingBottomPi = 65,
		//(Enum Bits:7,1,4)
		//equal to IntValue.9..9 Pi
		ApproachingTopPi = 73,
		#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5,1)
		ApproachingMidLeftPi = 81,
		//(Enum Bits:7,4,5,1)
		ApproachingMidRightPi = 89,
		#endif
	#endif
#endif
#if defined(AltNum_EnableERep)
		ENum = 2,
	#if defined(AltNum_EnablePowerOfRepresentation)
		EPower = 18,
	#endif
	#if defined(AltNum_EnableFractionals)
		ENumByDiv = 10,//(Value/(ExtraRep.Value))*e Representation
	#endif
	#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7,2)
		//equal to IntValue.0..01 e
		ApproachingBottomE = 66,
		//(Enum Bits:7,2)
		//equal to IntValue.9..9 e
		ApproachingTopE = 74,
		#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5,2)
		ApproachingMidLeftE = 82,
		//(Enum Bits:7,4,5,2)
		ApproachingMidRightE = 90,
		#endif
	#endif
#endif
#if defined(AltNum_EnableIRep)
		INum = 4,
	#if defined(AltNum_EnableFractionals)
		//(Value/(ExtraRep.Value))*i Representation
		INumByDiv = 11,
	#endif
#endif
#if defined(AltNum_EnableMixedFractional)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedFrac = 32,
	#if defined(AltNum_EnablePiRep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedPi = 33,
	#endif
	#if defined(AltNum_EnableERep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedE = 34,
	#endif
	#if defined(AltNum_EnableIRep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedI = 36,
	#endif
#endif

#if defined(AltNum_EnableInfinityRep)
		//(Enum Bits:7,6)
		//If Positive Infinity, then convert number into MaximumValue instead when need as real number
		//If Negative Infinity, then convert number into MinimumValue instead when need as real number
		Infinity = 96,
#endif
#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7)
		ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
		//(Enum Bits:7,4)
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		ApproachingTop = 72,
	#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5)
		//DecimalHalf:1000000000/ExtraRep - ApproachingZero (AlternativeName:ApproachingMidLeft)
		ApproachingMidLeft = 80,
		//(Enum Bits:7,4,5)
		//DecimalHalf:1000000000/ExtraRep + ApproachingZero (AlternativeName:ApproachingMidRight)
		ApproachingMidRight = 88,
	#endif
#endif
#if defined(AltNum_EnableImaginaryInfinity)
		//(Enum Bits:7,6,3)
		ImaginaryInfinity = 100,
#endif
#if defined(AltNum_EnableApproaching)&&defined(AltNum_EnableIRep)
		//(Enum Bits:7,3)
		//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		ApproachingImaginaryBottom = 68,
		//(Enum Bits:7,3,4)
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		ApproachingImaginaryTop = 76,
	#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,3,5)
		//DecimalHalf:1000000000/ExtraRep - ApproachingImaginaryZero
		ApproachingImaginaryMidLeft = 84,
		//(Enum Bits:7,3,5,4)
		//DecimalHalf:1000000000/ExtraRep + ApproachingImaginaryZero
		ApproachingImaginaryMidRight = 92,
	#endif
#endif
#if defined(AltNum_EnableNaN)
		//(Enum Bits:8)
		Undefined = 128,
		//(Enum Bits:8, 1)
		NaN = 129,
#endif
#if defined(AltNum_EnableUndefinedButInRange)
		//(Enum Bits:8, 2)
		//Such as result of Cos of infinity(value format part uses for +- range, DecimalHalf==UndefinedInRangeRep)
		UndefinedButInRange = 130,
	#if defined(AltNum_EnableWithinMinMaxRange)
		//(Enum Bits:8,6)
		//Undefined except for ranged IntValue to DecimalHalf (ExtraRep.Value==0; IsAltRep==1 when right side is negative)
		WithinMinMaxRange = 160,
		#if defined(AltNum_EnablePiRep)
		//(Enum Bits:8,6,1)
		WithinMinMaxRangePi = 161,
		#endif
		#if defined(AltNum_EnableERep)
		//(Enum Bits:8,6,2)
		WithinMinMaxRangeE = 162,
		#endif
		#if defined(AltNum_EnableIRep)
		//(Enum Bits:8,6,3)
		WithinMinMaxRangeI = 164,
		#endif
	#endif
#endif
#if defined(AltNum_EnableNil)
		//(Enum Bits:8, 1, 2)
		Nil = 131,
#endif
#if defined(AltNum_EnableIndeterminateForms)
		//0/0, infinity/infinity, 0 to power of 0, 0 * infinity, infinity - infinity, 1 to power of infinity, and infinity to power of 0
		//(Enum Bits:8,4)
		//Defined when DecimalHalf.Value == IndeterminateRep with exact value determined by IntValue field
		IndeterminateForm = 136,
#endif
#if defined(AltNum_EnableEnhancedDivideByZeroForm)
		//Indeterminate form of divided by zero
		//(Enum Bits:8,5)
		DividedByZero = 144,
#endif
#if defined(AltNum_EnableTrigExpressions)
		//(Enum Bits:9)
		Sin = 256,
		#if defined(AltNum_EnablePiRep)
		SinPi = 257,
		#endif
		#if defined(AltNum_EnableERep)
		SinE = 258,
		#endif
		#if defined(AltNum_EnableIRep)
		SinI = 260,
		#endif
		//(Enum Bits:10)
		Cos = 512,
		#if defined(AltNum_EnablePiRep)
		CosPi = 513,
		#endif
		#if defined(AltNum_EnableERep)
		CosE = 514,
		#endif
		#if defined(AltNum_EnableIRep)
		CosI = 516,
		#endif
		//(Enum Bits:11)
		Tan = 1024,
		#if defined(AltNum_EnablePiRep)
		TanPi = 1025,
		#endif
		#if defined(AltNum_EnableERep)
		TanE = 1026,
		#endif
		#if defined(AltNum_EnableIRep)
		TanI = 1028,
		#endif
#endif
	#ifdef AltNum_EnableComplexNumbers
		//Enum Bits subject to change for Complec Number later(Not completely used yet)
		ComplexIRep = 255,
	#endif
	
		//Not currently used(Enum Bits:8, 1, 2, 3)
		UnknownType = 135
	};
	
#if !defined(AltNum_UseJustEnumForRepType)
	class RepType
	{
	public:
		//Stored enum value for RepType
		RepTypeEnum Value;
	
		RepType(const RepType& value)
		{
			Value = value;
		}
	
        RepType& operator=(const RepType& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs.Value;
            return *this;
        } const
		
        RepType& operator=(const RepTypeEnum& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs;
            return *this;
        } const
	
		bool operator==(const RepType& that) const
		{
			Value==that.Value;
		}
		
		bool operator!=(const RepType& that) const
		{
			Value!=that.Value;
		}
		
		static const RepType NormalType = RepTypeEnum::NormalType;
#if defined(AltNum_EnableFractionals)
		static const RepType NumByDiv = RepTypeEnum::NumByDiv;
#endif
#if defined(AltNum_EnablePowerOfRepresentation)
		static const RepType ToPowerOf = RepTypeEnum::ToPowerOf;
#endif
#if defined(AltNum_EnablePiRep)
		static const RepType PiNum = RepTypeEnum::PiNum;
	#if defined(AltNum_EnablePowerOfRepresentation)
		static const RepType PiPower = RepTypeEnum::PiPower;
	#endif
	#if defined(AltNum_EnableFractionals)
		static const RepType PiNumByDiv = RepTypeEnum::PiNumByDiv;
	#endif
	#if defined(AltNum_EnableApproaching)
		static const RepType ApproachingBottomPi = RepTypeEnum::ApproachingBottomPi;
		static const RepType ApproachingTopPi = RepTypeEnum::ApproachingTopPi;
		#if defined(AltNum_EnableApproachingDivided)
		static const RepType ApproachingMidLeftPi = RepTypeEnum::ApproachingMidLeftPi;
		static const RepType ApproachingMidRightPi = RepTypeEnum::ApproachingMidRightPi;
		#endif
	#endif
#endif
#if defined(AltNum_EnableERep)
		static const RepType ENum = RepTypeEnum::ENum;
	#if defined(AltNum_EnablePowerOfRepresentation)
		static const RepType EPower = RepTypeEnum::EPower;
	#endif
	#if defined(AltNum_EnableFractionals)
		static const RepType ENumByDiv = RepTypeEnum::ENumByDiv;
	#endif
	#if defined(AltNum_EnableApproaching)
		static const RepType ApproachingBottomE = RepTypeEnum::ApproachingBottomE;
		static const RepType ApproachingTopE = RepTypeEnum::ApproachingTopE;
		#if defined(AltNum_EnableApproachingDivided)
		static const RepType ApproachingMidLeftE = RepTypeEnum::ApproachingMidLeftE;
		static const RepType ApproachingMidRightE = RepTypeEnum::ApproachingMidRightE;
		#endif
	#endif
#endif
#if defined(AltNum_EnableIRep)
		static const RepType INum = RepTypeEnum::INum;
	#if defined(AltNum_EnableFractionals)
		static const RepType INumByDiv = RepTypeEnum::INumByDiv;
	#endif
#endif
#if defined(AltNum_EnableMixedFractional)
		static const RepType MixedFrac = RepTypeEnum::MixedFrac;
	#if defined(AltNum_EnablePiRep)
		static const RepType MixedPi = RepTypeEnum::MixedPi;
	#endif
	#if defined(AltNum_EnableERep)
		static const RepType MixedE = RepTypeEnum::MixedE;
	#endif
	#if defined(AltNum_EnableIRep)
		static const RepType MixedI = RepTypeEnum::MixedI;
	#endif
#endif

#if defined(AltNum_EnableInfinityRep)
		static const RepType Infinity = RepTypeEnum::Infinity;
#endif
#if defined(AltNum_EnableApproaching)
		static const RepType ApproachingBottom = RepTypeEnum::ApproachingBottom;
		static const RepType ApproachingTop = RepTypeEnum::ApproachingTop;
	#if defined(AltNum_EnableApproachingDivided)
		static const RepType ApproachingMidLeft = RepTypeEnum::ApproachingMidLeft;
		static const RepType ApproachingMidRight = RepTypeEnum::ApproachingMidRight;
	#endif
#endif
#if defined(AltNum_EnableImaginaryInfinity)
		static const RepType ImaginaryInfinity = RepTypeEnum::ImaginaryInfinity;
#endif
#if defined(AltNum_EnableApproaching)&&defined(AltNum_EnableIRep)
		static const RepType ApproachingImaginaryBottom = RepTypeEnum::ApproachingImaginaryBottom;
		static const RepType ApproachingImaginaryTop = RepTypeEnum::ApproachingImaginaryTop;
	#if defined(AltNum_EnableApproachingDivided)
		static const RepType ApproachingImaginaryMidLeft = RepTypeEnum::ApproachingImaginaryMidLeft;
		static const RepType ApproachingImaginaryMidRight = RepTypeEnum::ApproachingImaginaryMidRight;
	#endif
#endif
#if defined(AltNum_EnableNaN)
		static const RepType Undefined = RepTypeEnum::Undefined;
		static const RepType NaN = RepTypeEnum::NaN;
#endif
#if defined(AltNum_EnableUndefinedButInRange)
		static const RepType UndefinedButInRange = RepTypeEnum::UndefinedButInRange;
	#if defined(AltNum_EnableWithinMinMaxRange)
		static const RepType WithinMinMaxRange = RepTypeEnum::WithinMinMaxRange;
		#if defined(AltNum_EnablePiRep)
		static const RepType WithinMinMaxRangePi = RepTypeEnum::WithinMinMaxRangePi;
		#endif
		#if defined(AltNum_EnableERep)
		static const RepType WithinMinMaxRangeE = RepTypeEnum::WithinMinMaxRangeE;
		#endif
		#if defined(AltNum_EnableIRep)
		static const RepType WithinMinMaxRangeI = RepTypeEnum::WithinMinMaxRangeI;
		#endif
	#endif
#endif
#if defined(AltNum_EnableNil)
		static const RepType Nil = RepTypeEnum::Nil;
#endif
#if defined(AltNum_EnableIndeterminateForms)
		//0/0, infinity/infinity, 0 to power of 0, 0 * infinity, infinity - infinity, 1 to power of infinity, and infinity to power of 0
		//Defined when DecimalHalf.Value == IndeterminateRep with exact value determined by IntValue field
		static const RepType IndeterminateForm = RepTypeEnum::IndeterminateForm;
#endif
#if defined(AltNum_EnableEnhancedDivideByZeroForm)
		//Indeterminate form of divided by zero
		static const RepType DividedByZero = RepTypeEnum::DividedByZero;
#endif
		//https://en.wikipedia.org/wiki/Sine_and_cosine
		//Unknown X value when IntHalf = NegativeZero
#if defined(AltNum_EnableTrigExpressions)//Store values as Formula IntHalf (with ExtraRep used for powers and divisors)
		//Sin(X or IntHalf (Radians));
		static const RepType Sin = RepTypeEnum::Sin;
		#if defined(AltNum_EnablePiRep)
		//Sin((X or IntHalf)Pi (Radians));
		static const RepType SinPi = RepTypeEnum::SinPi;
		#endif
		#if defined(AltNum_EnableERep)
		static const RepType SinE = RepTypeEnum::SinE;
		#endif
		#if defined(AltNum_EnableIRep)
		static const RepType SinI = RepTypeEnum::SinI;
		#endif
		//(Enum Bits:10)
		static const RepType Cos = RepTypeEnum::Cos;
		#if defined(AltNum_EnablePiRep)
		static const RepType CosPi = RepTypeEnum::CosPi;
		#endif
		#if defined(AltNum_EnableERep)
		static const RepType CosE = RepTypeEnum::CosE;
		#endif
		#if defined(AltNum_EnableIRep)
		static const RepType CosI = RepTypeEnum::CosI;
		#endif
		//(Enum Bits:11)
		static const RepType Tan = RepTypeEnum::Tan;
		#if defined(AltNum_EnablePiRep)
		static const RepType TanPi = RepTypeEnum::TanPi;
		#endif
		#if defined(AltNum_EnableERep)
		static const RepType TanE = RepTypeEnum::TanE;
		#endif
		#if defined(AltNum_EnableIRep)
		static const RepType TanI = RepTypeEnum::TanI;
		#endif
#endif

		static std::string RepTypeAsString(RepType& repType)
		{
			switch(repType)
			{
				case RepType::NormalType:
					return "NormalType"; break;
	#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:
					return "NumByDiv"; break;
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::NumByDiv:
					return "NumByDiv"; break;
	#endif
	#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
					return "PiNum"; break;
		#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::PiPower:
					return "PiPower"; break;
		#endif
		#if defined(AltNum_EnableFractionals)
				case RepType::PiNumByDiv:
					return "PiNumByDiv"; break;
		#endif
		#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomPi:
					return "ApproachingBottomPi"; break;
				case RepType::ApproachingTopPi:
					return "ApproachingTopPi"; break;
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftPi:
					return "ApproachingMidLeftPi"; break;
				case RepType::ApproachingMidRightPi:
					return "ApproachingMidRightPi"; break;
			#Pindif
		#Pindif
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
					return "ENum"; break;
		#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::EPower:
					return "EPower"; break;
		#endif
		#if defined(AltNum_EnableFractionals)
				case RepType::ENumByDiv:
					return "ENumByDiv"; break;
		#endif
		#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomE:
					return "ApproachingBottomE"; break;
				case RepType::ApproachingTopE:
					return "ApproachingTopE"; break;
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftE:
					return "ApproachingMidLeftE"; break;
				case RepType::ApproachingMidRightE:
					return "ApproachingMidRightE"; break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac:
					return "MixedFrac"; break;
		#if defined(AltNum_EnablePiRep)
				case RepType::MixedPi:
					return "MixedPi"; break;
		#endif
		#if defined(AltNum_EnableERep)
				case RepType::MixedE:
					return "MixedE"; break;
		#endif
		#if defined(AltNum_EnableIRep)
				case RepType::MixedI:
					return "MixedI"; break;
		#endif
	#endif
	#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    return "ApproachingBottom"; break;
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    return "ApproachingTop"; break;
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:
					return "ApproachingMidLeft"; break;
				case RepType::ApproachingMidRight:
					return "ApproachingMidRight"; break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
				case RepType::Infinity:
					return "Infinity"; break;
	#endif
	#if defined(AltNum_EnableIRep)
				case RepType::INum:
                    return "INum"; break;
		#if defined(AltNum_EnableFractionals)
				case RepType::INumByDiv:
					return "INumByDiv"; break;
		#endif
		#if defined(AltNum_EnableInfinityRep)
				case RepType::ImaginaryInfinity:
					return "ImaginaryInfinity"; break;
		#endif
		#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					return "ApproachingImaginaryBottom"; break;
				case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					return "ApproachingImaginaryTop"; break;
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:
					return "ApproachingImaginaryMidLeft"; break;
				case RepType::ApproachingImaginaryMidRight:
					return "ApproachingImaginaryMidRight"; break;
			#endif
    #endif
		#ifdef AltNum_EnableComplexNumbers
				case RepType::ComplexIRep:
					return "ComplexIRep"; break;
		#endif
	#endif
    #if defined(AltNum_EnableNaN)
				case RepType::Undefined:
					return "Undefined"; break;
				case RepType::NaN:
					return "NaN"; break;
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)
				case RepType::UndefinedButInRange:
					return "UndefinedButInRange"; break;
		#if defined(AltNum_EnableWithinMinMaxRange)
				case RepType::WithinMinMaxRange:
					return "WithinMinMaxRange"; break;
			#if defined(AltNum_EnablePiRep)
			#endif
			#if defined(AltNum_EnableERep)
			#endif
			#if defined(AltNum_EnableIRep)
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableTrigExpressions)
		#if defined(AltNum_EnablePiRep)
		#endif
		#if defined(AltNum_EnableERep)
		#endif
		#if defined(AltNum_EnableIRep)
		#endif
	#endif
    #if defined(AltNum_EnableNilRep)
				case RepType::Nil:
					return "Nil"; break;
    #endif
				default:
					return "Unknown";
			}
		}

	};
#endif
}