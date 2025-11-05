// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//#include "..\DLLAPI.h"
#include <cstdint>
#include <concepts>
#include "BinaryTypeSelector.hpp"//For selecting types based on bit total
#include "RepTypeDetection.hpp"

namespace BlazesRusCode
{
  using namespace RepTypeDetection,
	/*
    // RepType: Enumerates the *current* representation type of a value.
    // Describes its canonical stored form after normalisation — not the
    // sequence of operations that produced it.
    // BitPositions above actual RepType enum masks are stored here as well
    // (Positions based on RestrictedFloat)
		//Layout is generated inside Layout as constexpr computation
		RepTypeV3 overview:

		Core constant multipliers
		(NormalType, PiNum, ENum, PiENum, INum)
								 → These identify the base constant family.
		Fractional & Approaching families:
								 - "ByDiv" — value is divided by ExtraRep (requires ExtraRep > 1)
								 - Other bits: ApproachingTop, ApproachingBottom, MidLeft, MidRight, etc.
									 Approaching forms may combine with ByDiv.

		Infinity, PowerOf, MixedFrac, Range checks:
								 → Special magnitude categories and numeric form modifiers.

	  Fixed magnitude ± (ApproachingBottom || ApproachingTop):
	    → Describes the current stored form as a fixed-point magnitude with a
	     saved infinitesimal offset term derived from an approaching form.
	     This is a representation descriptor, not an operation history.
	
    Fixed(MediumDec variant related) NormalNum(and related before multiplier Notes:
	     Fixed value:
	       value_fixed = IntHalf + DecimalHalf / DecimalOverflow
	     Smallest fixed unit:
	       ε = 1 / DecimalOverflow
	       (if ByDiv is set: ε = 1 / (DecimalOverflow * ExtraRep))
	
	     Examples (using DecimalHalf explicitly):
	       2.4 + 0.00…01 → value_fixed + ε
	         → PlusOffset, ExtraRep sentinel = ApproachingBottom
	
	       2.4 + 0.99…9 → (IntHalf + 1) + DecimalHalf/DecimalOverflow − ε
	         → Represented as Fixed + ApproachingTop
	         → PlusOffset, ExtraRep sentinel = ApproachingTop
	
	     Notes:
	     - PlusOffset literally means “Fixed + saved offset term.”
	     - Values that could be re-expressed as MinusOffset are not normalized
	       ahead of time, representation reflects current stored form.
	
	     PlusOffset  (Fixed + ApproachingX(...))
	     MinusOffset (Fixed − ApproachingX(...))

		IndeterminateForm:
				→ Given priority over other flags, subtype bits may also be set.

		Trig expression flags:
			  → Symbolic trig forms (sin, cos, tan, etc.) stored in ExtraRep.

    Phi (φ) = (1 + sqrt(5)) / 2:
    //≈ 1.618033988749894848204586834365638117720309179805762862135448622705260462818902449707207204189391137...
    // The golden ratio, often used in geometry and number theory.
		// Special properties:
		//   - Reciprocal: 1/φ = φ - 1
		//   - Companion constant: sqrt(5) (φ is defined in terms of it)
		//   - Fibonacci link: F_n = (φ^n - (1-φ)^n)/sqrt(5)
		//   - closed form growth constant of the Fibonacci sequence
		// These relationships may justify treating φ, 1/φ, and sqrt(5)
		// as a coupled family of symbolic constants.

		PhiConjugateNum represents N × (1/φ):
		// Algebraic identity: 1/φ = φ − 1 (the conjugate of φ)

    Plastic constant(ρ) == ≈1.32471795724474602596090885447809734:
    //https://oeis.org/wiki/Plastic_constant
		//real root of (x^3 = x + 1)e
		// PlasticSquareNum * PlasticNum is  reduced to (ρ + 1).
		//ρ is tied to Padovan and Perrin sequences
		
		//For imaginary variants of any representation represents: (NormalFormula)*imaginary number
    // includes UnitRangedMode's 1.0 (to reduce duplicate enum representations)
	*/
  template<class Policy, class Layout>
  enum class FullRepTypeV3 : FullRepUnderlying<Layout> {
		// Unknown category used to remove unused enum features
    // No real category will be assigned to this slot.
		UnknownType  = std::numeric_limits<RepUnderlying<Layout>>::max(),
		
    //Used both for ZeroSentinal and zero based combination Representations like +-0.0..01
		ZeroSentinal = Layout::ZeroSentinalRep,
		PositiveSign = Policy::SignedMode ? (Policy::PositiveSign ? SignRep : 0) : 0,
		NegativeSign = Policy::SignedMode ? (Policy::NegativeSign ? SignRep : 0) : UnknownType,
	  //At or Above one
    AboveSub = Policy::SignedExpMode ? Layout::ExpSignRep : UnknownType,
	
		PositiveZero = Has_SignedZero<Policy>?PositiveSign|ZeroSentinal : UnknownType,
		NegativeZero = Has_SignedZero<Policy>?NegativeSign|ZeroSentinal : UnknownType,
	
#pragma region  NormalType

		PositiveAboveSubNormalType = Policy::SignedExpMode ? PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubNormalType = Policy::SignedExpMode&&Policy::SignedMode ? NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeNormalType = PositiveSign,
		NegativeSubRangeNormalType = Policy::SignedMode ? NegativeSign : UnknownType,

#pragma endregion  NormalType

#pragma region  Core multipliers

		PositiveAboveSubPiNum = PiRepresented<Layout>&&Policy::SignedExpMode ? Layout::PiNumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPiNum = PiRepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::PiNumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePiNum = PiRepresented<Layout>? Layout::PiNumRep|PositiveSign : UnknownType,
		NegativeSubRangePiNum = PiRepresented<Layout>&&Policy::SignedMode ? Layout::PiNumRep|NegativeSign : UnknownType,

		PositiveAboveSubENum = ERepresented<Layout>&&Policy::SignedExpMode ? Layout::ENumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubENum = ERepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::ENumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeENum = ERepresented<Layout>? Layout::ENumRep|PositiveSign : UnknownType,
		NegativeSubRangeENum = ERepresented<Layout>&&Policy::SignedMode ? Layout::ENumRep|NegativeSign : UnknownType,

		PositiveAboveSubINum = IRepresented<Layout>&&Policy::SignedExpMode ? Layout::INumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubINum = IRepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::INumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeINum = IRepresented<Layout>? Layout::INumRep|PositiveSign : UnknownType,
		NegativeSubRangeINum = IRepresented<Layout>&&Policy::SignedMode ? Layout::INumRep|NegativeSign : UnknownType,

#pragma endregion  Core multipliers

#pragma region  Core composite representations

		PositiveAboveSubPiENum = PiERepresented<Layout>&&Policy::SignedExpMode ? Layout::PiENumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPiENum = PiERepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::PiENumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePiENum = PiERepresented<Layout>? Layout::PiENumRep|PositiveSign : UnknownType,
		NegativeSubRangePiENum = PiERepresented<Layout>&&Policy::SignedMode ? Layout::PiENumRep|NegativeSign : UnknownType,

		PositiveAboveSubEINum = EIRepresented<Layout>&&Policy::SignedExpMode ? Layout::EINumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubEINum = EIRepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::EINumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeEINum = EIRepresented<Layout>? Layout::EINumRep|PositiveSign : UnknownType,
		NegativeSubRangeEINum = EIRepresented<Layout>&&Policy::SignedMode ? Layout::EINumRep|NegativeSign : UnknownType,

		PositiveAboveSubPiINum = PiIRepresented<Layout>&&Policy::SignedExpMode ? Layout::PiINumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPiINum = PiIRepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::PiINumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePiINum = PiIRepresented<Layout>? Layout::PiINumRep|PositiveSign : UnknownType,
		NegativeSubRangePiINum = PiIRepresented<Layout>&&Policy::SignedMode ? Layout::PiINumRep|NegativeSign : UnknownType,

		PositiveAboveSubPiEINum = PiEIRepresented<Layout>&&Policy::SignedExpMode ? Layout::PiEINumRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPiEINum = PiEIRepresented<Layout>&&Policy::SignedExpMode&&Policy::SignedMode ? Layout::PiEINumRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePiEINum = PiEIRepresented<Layout>? Layout::PiEINumRep|PositiveSign : UnknownType,
		NegativeSubRangePiEINum = PiEIRepresented<Layout>&&Policy::SignedMode ? Layout::PiEINumRep|NegativeSign : UnknownType,

#pragma endregion  Core composite representations

#pragma endregion  Fractional representations

	  PositiveAboveSubNumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::NumByDivRep|PositiveSign| AboveSub : UnknownType,
	  NegativeAboveSubNumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::NumByDivRep|NegativeSign| AboveSub : UnknownType,
	  PositiveSubRangeNumByDiv = ByDivRepresented<Layout>? Layout::NumByDivRep|PositiveSign : UnknownType,
	  NegativeSubRangeNumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout> ? Layout::NumByDivRep|NegativeSign : UnknownType,

	  PositiveAboveSubMixedFrac = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::MixedFracRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubMixedFrac = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::MixedFracRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeMixedFrac = MixedFracRepresented<Layout> ? Layout::MixedFracRep|PositiveSign : UnknownType,
		NegativeSubRangeMixedFrac = MixedFracRepresented<Layout>&&SignEnabled<Layout> ? Layout::MixedFracRep|NegativeSign : UnknownType,

#pragma endregion  Fractional representations

#pragma region  Approaching representations

		//(Approaching Towards Zero),((Raw as unsigned) of 0 results in 0.00...1)
    //Not allowed for Pi and E variants(because constant would vanish in infinite approaching slope)
    //RestrictedFloat reinterpretes raw as uint instead of exponental fraction
		PositiveApproachingBottom = ApproachingRepresented<Layout>? Layout::ApproachingBottomRep|PositiveSign : UnknownType,
		NegativeApproachingBottom = ApproachingRepresented<Layout>&&SignEnabled<Layout> ? Layout::ApproachingBottomRep|NegativeSign : UnknownType,

		//(Approaching Away from Zero),((Raw as unsigned) results in 0.99...9)
    //RestrictedFloat reinterpretes raw as uint instead of exponental fraction
		PositiveApproachingTop = ApproachingTopRepresented<Layout>? Layout::ApproachingTopRep|PositiveSign : UnknownType,
		NegativeApproachingTop = ApproachingTopRepresented<Layout>&&SignEnabled<Layout> ? Layout::ApproachingTopRep|NegativeSign : UnknownType,

#pragma region  ApproachingBottom representation divided by ExtraRep

    PositiveApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> ?
    Layout::ApproachingBottomByDivRep|PositiveSign : UnknownType,
		
    NegativeApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign : UnknownType,

#pragma region  ApproachingBottom representation divided by ExtraRep

#pragma region  ApproachingTop representation divided by ExtraRep

    PositiveApproachingTopByDiv = ApproachingTopByDivRepresented<Layout> ?
    Layout::ApproachingTopByDivRep|PositiveSign : UnknownType,
		
    NegativeApproachingTopByDiv = ApproachingTopByDivRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|Layout::NumByDivRep|NegativeSign : UnknownType,

#pragma endregion  ApproachingTop representation divided by ExtraRep

// Approaching midpoint from below (fractional part = 0.999… / ExtraRep)
// Requires EnableApproachingDivided and ExtraRep divisor.
// When ExtraRep == 1, numerically equals ApproachingTop.
// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)-0.0...01)
// For variant representations : equals (Normal formula)*constant
#pragma region  ApproachingMidLeft

		PositiveApproachingMidLeft = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> ?
    Layout::ApproachingMidLeftRep|PositiveSign : UnknownType,
		NegativeApproachingMidLeft = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout> ?
    Layout::ApproachingMidLeftRep|NegativeSign : UnknownType,

    //(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
		PositiveZeroApproachingMidLeft = ApproachingMidRepresented<Layout> ?
    Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal : UnknownType,
		NegativeZeroApproachingMidLeft = ApproachingMidRepresented<Layout> && SignEnabled<Layout> ?
    Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal : UnknownType,

#pragma endregion  ApproachingMidLeft

// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
#pragma region  ApproachingMidRight

		PositiveApproachingMidRight = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> ?
    Layout::ApproachingMidRightRep|PositiveSign : UnknownType,
		NegativeApproachingMidRight = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout> ?
    Layout::ApproachingMidRightRep|NegativeSign : UnknownType,

    //(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
		PositiveZeroApproachingMidRight = ApproachingMidRepresented<Layout> ?
    Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal : UnknownType,
		NegativeZeroApproachingMidRight = ApproachingMidRepresented<Layout> && SignEnabled<Layout> ?
    Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal : UnknownType,

#pragma endregion  ApproachingMidRight

// PlusOffset = +ε (approach from below), MinusOffset = −ε (approach from above).
#pragma region  ApproachingOffsets
    
		//SignMultiplier*(NormalNum magnitude) + ApproachingBottom)*(constant multiplier)
		PositiveApproachingBottomPlusOffset = ApproachingOffsetsRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep:UnknownType,
		NegativeApproachingBottomPlusOffset = ApproachingOffsetsRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep:UnknownType,
		
    //SignMultiplier*(NormalNum magnitude + ApproachingTop)*(constant multiplier)
		PositiveApproachingTopPlusOffset = ApproachingOffsetsRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep:UnknownType,
		NegativeApproachingTopPlusOffset = ApproachingOffsetsRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep:UnknownType,
		
    //SignMultiplier*(NormalNum magnitude - ApproachingBottom)*(constant multiplier)
    PositiveApproachingBottomMinusOffset = ApproachingOffsetsRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep:UnknownType,
    NegativeApproachingBottomMinusOffset = ApproachingOffsetsRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep:UnknownType,
		
    //SignMultiplier*(NormalNum magnitude - ApproachingBottom)*(constant multiplier)
    PositiveApproachingTopMinusOffset = ApproachingOffsetsRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep:UnknownType,
    NegativeApproachingTopMinusOffset = ApproachingOffsetsRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep:UnknownType,


#pragma endregion  ApproachingOffsets

#pragma endregion  Approaching representations

#pragma region  Infinite representations

		PositiveInfinity = InfinityRepresented<Layout> ? PositiveSign|Layout::InfinityRep : UnknownType,
    PositiveImaginaryInfinity = InfinityRepresented<Layout> ? PositiveSign|Layout::InfinityRep| Layout::INumRep : UnknownType,
		NegativeInfinity = InfinityRepresented<Layout> ? NegativeSign|Layout::InfinityRep : UnknownType,
    NegativeImaginaryInfinity = InfinityRepresented<Layout> ? NegativeSign|Layout::InfinityRep| Layout::INumRep : UnknownType,

#pragma endregion  Infinite representations

#pragma endregion  UniverseUnits representations

    //Bounded to maximum of either 1 magnitude(either wrap around behavior or exception if exceed)
		//Similar to infinity in that doesn't have measureable in real units length
    //but still acts as maximum theorical distance in a universe etc
    PositiveUniverseUnits = UniverseUnitsRepresented<Layout> ? PositiveSign|Layout::UniverseUnitsRep : UnknownType,
    PositiveImaginaryUniverseUnits = UniverseUnitsRepresented<Layout> ? PositiveSign|Layout::UniverseUnitsRep| Layout::INumRep : UnknownType,
		NegativeUniverseUnits = UniverseUnitsRepresented<Layout> ? NegativeSign|Layout::UniverseUnitsRep : UnknownType,
    NegativeImaginaryUniverseUnits = UniverseUnitsRepresented<Layout> ? NegativeSign|Layout::UniverseUnitsRep| Layout::INumRep : UnknownType,

#pragma endregion  UniverseUnits representations

#pragma region  PowerOf representations

		PositiveAboveSubPowerOf = PowerOfRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOf = PowerOfRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ?
    Layout::ToPowerOfRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOf = PowerOfRepresented<Layout>?
    Layout::ToPowerOfRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOf = PowerOfRepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|NegativeSign : UnknownType,

		PositiveAboveSubPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>? 
    Layout::ToPowerOfRep|Layout::PiNumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>?
    Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout::PiNumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout> ? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout> ? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign : UnknownType,
		
		PositiveAboveSubPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangePowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign : UnknownType,
		NegativeSubRangePowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign : UnknownType,

#pragma endregion  PowerOf representations

#pragma region  WithinMinMaxRange representations
/*
		//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		WithinMinMaxRange = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange ? Layout::WithinMinMaxRangeRep : UnknownType,
		//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		NotWithinMinMaxRange = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange ? Layout::NotWithinMinMaxRangeRep : UnknownType,
*/
#pragma endregion  WithinMinMaxRange representations

#pragma region Other math constants
/*
	  PhiNum    = Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::PhiNumRep : UnknownType,
		//√5
	  SqrtXNum    = Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::SqrtXNumRep : UnknownType,
		PhiConjugateNum  = Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::DividedByPhiNumRep : UnknownType,
    //ln(2)
	  LnXNum    = Has_EnableLnXNum<Policy>&&Policy::EnableLnX ? Layout::LnXRep : UnknownType
	  PlasticNum    = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum ? Layout::PlasticNumRep : UnknownType
	  PlasticSquareNum    = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum ? Layout::PlasticSquareNumRep : UnknownType
*/
#pragma endregion Other math constants

#pragma region  Experimental symbolic trig expression flags
		// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
  #pragma region  Circular symbolic trig expression flags

		PositiveAboveSubSin = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::SinRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubSin = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::SinRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeSin = TrigRepresented<Layout>?
    Layout::SinRep|PositiveSign : UnknownType,
		NegativeSubRangeSin = TrigRepresented<Layout>&&SignEnabled<Layout>?
    Layout::SinRep|NegativeSign : UnknownType,

		PositiveAboveSubCos = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::CosRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubCos = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::CosRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeCos = TrigRepresented<Layout>?
    Layout::CosRep|PositiveSign : UnknownType,
		NegativeSubRangeCos = TrigRepresented<Layout>&&SignEnabled<Layout>?
    Layout::CosRep|NegativeSign : UnknownType,

		PositiveAboveSubTan = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::TanRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubTan = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>?
    Layout::TanRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeTan = TrigRepresented<Layout>? Layout::TanRep|PositiveSign : UnknownType,
		NegativeSubRangeTan = TrigRepresented<Layout>&&SignEnabled<Layout>?
    Layout::TanRep|NegativeSign : UnknownType,

  #pragma endregion  Circular symbolic trig expression flags

  #pragma region  Hyperbolic symbolic expression flags

		PositiveAboveSubSinH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::SinHRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubSinH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::SinHRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeSinH = HyperbolicRepresented<Layout> ? Layout::SinHRep|PositiveSign : UnknownType,
		NegativeSubRangeSinH = HyperbolicRepresented<Layout>&&SignEnabled<Layout> ? Layout::SinHRep|NegativeSign : UnknownType,

		PositiveAboveSubCosH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::CosHRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubCosH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::CosHRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeCosH = HyperbolicRepresented<Layout> ? Layout::CosHRep|PositiveSign : UnknownType,
		NegativeSubRangeCosH = HyperbolicRepresented<Layout>&&SignEnabled<Layout> ? Layout::CosHRep|NegativeSign : UnknownType,

		PositiveAboveSubTanH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::TanHRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubTanH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::TanHRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeTanH = HyperbolicRepresented<Layout> ? Layout::TanHRep|PositiveSign : UnknownType,
		NegativeSubRangeTanH = HyperbolicRepresented<Layout>&&SignEnabled<Layout> ? Layout::TanHRep|NegativeSign : UnknownType,

  #pragma endregion  Hyperbolic symbolic expression flags

  #pragma region  Inversed expressions

    //Inversed Circular symbolic expression flag
		PositiveAboveSubInversedOf = InversionRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::InversedOfRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubInversedOf = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::InversedOfRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeInversedOf = InversionRepresented<Layout>? Layout::InversedOfRep|PositiveSign : UnknownType,
		NegativeSubRangeInversedOf = InversionRepresented<Layout>&&SignEnabled<Layout> ? Layout::InversedOfRep|NegativeSign : UnknownType,

    #pragma region  ArcSin
		PositiveAboveSubCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep : UnknownType,
		NegativeAboveSubCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep : UnknownType,
		PositiveSubRangeCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::SinRep : UnknownType,
		NegativeSubRangeCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign | Layout::SinRep : UnknownType,
    #pragma endregion  ArcSin

    #pragma region  ArcCos
		PositiveAboveSubArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep : UnknownType,
		NegativeAboveSubArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep: UnknownType,
		PositiveSubRangeArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::CosRep: UnknownType,
		NegativeSubRangeArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign | Layout::CosRep: UnknownType,
    #pragma endregion  ArcCos

    #pragma region  ArcTan
		PositiveAboveSubArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep: UnknownType,
		NegativeAboveSubArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep: UnknownType,
		PositiveSubRangeArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::TanRep: UnknownType,
		NegativeSubRangeArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign | Layout::TanRep: UnknownType,
    #pragma region  ArcTan

    #pragma region  Hyperbolic ArcSin
		PositiveAboveSubCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep : UnknownType,
		NegativeAboveSubCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep : UnknownType,
		PositiveSubRangeCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::SinHRep : UnknownType,
		NegativeSubRangeCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
    ? Layout::InversedOfRep|NegativeSign | Layout::SinHRep : UnknownType,
    #pragma endregion  Hyperbolic ArcSin

    #pragma region  Hyperbolic ArcCos
		PositiveAboveSubArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep : UnknownType,
		NegativeAboveSubArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep: UnknownType,
		PositiveSubRangeArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::CosHRep: UnknownType,
		NegativeSubRangeArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign | Layout::CosHRep: UnknownType,
    #pragma endregion  Hyperbolic ArcCos

    #pragma region  Hyperbolic ArcTan
		PositiveAboveSubArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep: UnknownType,
		NegativeAboveSubArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep: UnknownType,
		PositiveSubRangeArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::TanHRep: UnknownType,
		NegativeSubRangeArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::InversedOfRep|NegativeSign | Layout::TanHRep: UnknownType,
    #pragma endregion  Hyperbolic ArcTan

  #pragma endregion  Inversed expressions

  #pragma region  Reciprocal expressions

		//Reciprocal expression flag 1/OtherExpression/Value
		PositiveAboveSubReciprocalOf = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::ReciprocalOfRep|PositiveSign| AboveSub : UnknownType,
		NegativeAboveSubReciprocalOf = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> ? Layout::ReciprocalOfRep|NegativeSign| AboveSub : UnknownType,
		PositiveSubRangeReciprocalOf = ReciprocalRepresented<Layout> ? Layout::ReciprocalOfRep|PositiveSign : UnknownType,
		NegativeSubRangeReciprocalOf = ReciprocalRepresented<Layout>&&SignEnabled<Layout> ? Layout::ReciprocalOfRep|NegativeSign : UnknownType,

    //cosecant(x) == 1/sin(x)
    #pragma region  cosecant
		PositiveAboveSubCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep : UnknownType,
		NegativeAboveSubCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep : UnknownType,
		PositiveSubRangeCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep : UnknownType,
		NegativeSubRangeCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep : UnknownType,
    #pragma endregion  cosecant

    //secant(x) = 1/cos(x)
    #pragma region  secant
		PositiveAboveSubSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep : UnknownType,
		NegativeAboveSubSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep: UnknownType,
		PositiveSubRangeSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep: UnknownType,
		NegativeSubRangeSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep: UnknownType,
    #pragma endregion  secant

    //Cotangent = 1/tan(x)
    #pragma region  Cotangent
		PositiveAboveSubCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep: UnknownType,
		NegativeAboveSubCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep: UnknownType,
		PositiveSubRangeCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep: UnknownType,
		NegativeSubRangeCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep: UnknownType,
    #pragma region  Cotangent

    #pragma region  Hyperbolic cosecant
		PositiveAboveSubCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep : UnknownType,
		NegativeAboveSubCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep : UnknownType,
		PositiveSubRangeCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep : UnknownType,
		NegativeSubRangeCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep : UnknownType,
    #pragma endregion  Hyperbolic cosecant

    #pragma region  Hyperbolic secant
		PositiveAboveSubSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep : UnknownType,
		NegativeAboveSubSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep: UnknownType,
		PositiveSubRangeSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep: UnknownType,
		NegativeSubRangeSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep: UnknownType,
    #pragma endregion  Hyperbolic secant

    #pragma region  Hyperbolic Cotangent
		PositiveAboveSubCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep: UnknownType,
		NegativeAboveSubCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep: UnknownType,
		PositiveSubRangeCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep: UnknownType,
		NegativeSubRangeCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep: UnknownType,
    #pragma region  Hyperbolic Cotangent

  #pragma endregion  Reciprocal expressions

  #pragma region  Arc cosecant
		PositiveAboveSubArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep : UnknownType,
		NegativeAboveSubArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep : UnknownType,
		PositiveSubRangeArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep : UnknownType,
		NegativeSubRangeArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep : UnknownType,
  #pragma endregion  Arc cosecant

  #pragma region  Arc secant
		PositiveAboveSubArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep : UnknownType,
		NegativeAboveSubArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep: UnknownType,
		PositiveSubRangeArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep: UnknownType,
		NegativeSubRangeArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep: UnknownType,
  #pragma endregion  ArcCos

  #pragma region  Arc cotangent
		PositiveAboveSubArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep: UnknownType,
		NegativeAboveSubArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep: UnknownType,
		PositiveSubRangeArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep: UnknownType,
		NegativeSubRangeArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep: UnknownType,
  #pragma region  Arc cotangent

  #pragma region  Hyperbolic Arc cosecant
		PositiveAboveSubCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep : UnknownType,
		NegativeAboveSubCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
    ? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep : UnknownType,
		PositiveSubRangeCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep|PositiveSign | Layout::SinHRep : UnknownType,
		NegativeSubRangeCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
    ? Layout::InversedOfRep|NegativeSign | Layout::SinHRep : UnknownType,
  #pragma endregion  Hyperbolic ArcSin

  #pragma region  Hyperbolic Arc secant
		PositiveAboveSubArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep : UnknownType,
		NegativeAboveSubArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep: UnknownType,
		PositiveSubRangeArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep: UnknownType,
		NegativeSubRangeArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep: UnknownType,
  #pragma endregion  Hyperbolic Arc secant

  #pragma region  Hyperbolic Arc cotangent
		PositiveAboveSubArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep: UnknownType,
		NegativeAboveSubArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep: UnknownType,
		PositiveSubRangeArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep: UnknownType,
		NegativeSubRangeArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep: UnknownType,
  #pragma region  Hyperbolic Arc cotangent

#pragma endregion  Experimental symbolic trig expression flags

#pragma region  Indeterminate forms
	  // --------------------
    // Indeterminate forms(Required to be highest bit value from Layout)
    // Requires enough bits set in layout in order for all Indeterminate forms to be enabled
    // --------------------
    // All share the IndeterminateForm bit(These act as sentinal not just as flags)
    // Additional bits indicate the specific undefined/indeterminate case.
    // Checked first when EnableIndeterminateForms is enabled.
		IndeterminateForm = Layout::IndeterminateFormRep!=0 ? Layout::IndeterminateFormRep : UnknownType,
		//NaN = Layout::IndeterminateFormRep!=0&&(Layout::NaNRep > IndeterminateFormRep) ? Layout::NaNRep : UnknownType,
		Nil = Layout::IndeterminateFormRep!=0&&(Layout::NilRep > IndeterminateFormRep) ? Layout::NilRep : UnknownType,
		Undefined = Layout::IndeterminateFormRep!=0&&(Layout::UndefinedRep > IndeterminateFormRep) ? Layout::UndefinedRep : UnknownType,
		DividedByZero = Layout::IndeterminateFormRep!=0&&(Layout::DividedByZeroRep > IndeterminateFormRep) ? Layout::DividedByZeroRep : UnknownType,
    //Zero divided by Zero
		ZeroByZero = Layout::IndeterminateFormRep!=0&&(Layout::ZeroByZeroRep > IndeterminateFormRep) ? Layout::ZeroByZeroRep : UnknownType,
		ZeroToZeroPower = Layout::IndeterminateFormRep!=0&&(Layout::ZeroToZeroPowerRep > IndeterminateFormRep) ? Layout::ZeroToZeroPowerRep : UnknownType,
    //∞/∞ Representation
		InfinityByInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityByInfinityRep > IndeterminateFormRep) ? Layout::InfinityByInfinityRep|PositiveSign : UnknownType,
		NegativeInfinityByInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityByInfinityRep > IndeterminateFormRep)&&Policy::SignedMode
    ? Layout::InfinityByInfinityRep|NegativeSign : UnknownType,
		
    InfinityMinusInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityMinusInfinityRep > IndeterminateFormRep) ? Layout::InfinityMinusInfinityRep : UnknownType,

		//∞^∞ Representation
		//InfinityPowerOfInfinity ∞^∞ — extreme divergence, useful to trap before overflow.
		InfinityPowerOfInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityPowerOfInfinityRep > IndeterminateFormRep) ? Layout::InfinityPowerOfInfinityRep : UnknownType,
    
    InfinityPowerOfNegativeInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityPowerOfNegativeInfinityRep > IndeterminateFormRep) &&Policy::SignedMode
    ? Layout::InfinityPowerOfNegativeInfinityRep : UnknownType,

		//NegativeOnePowerOfInfinity represented if Sign bit is negative
		//NegativeOnePowerOfInfinity (-1)^∞ — oscillates between ±1,
		NegativeOnePowerOfInfinity = Layout::IndeterminateFormRep!=0&&(Layout::NegativeOnePowerOfInfinity > IndeterminateFormRep) ? Layout::NegativeOnePowerOfInfinity : UnknownType,
    
    //OnePowerOfInfinity = Layout::IndeterminateFormRep!=0&&(Layout::OnePowerOfInfinityRep > IndeterminateFormRep) ? Layout::OnePowerOfInfinityRep : UnknownType,
//		InfinityToZeroPower = Layout::IndeterminateFormRep!=0&&(Layout::InfinityToZeroPowerRep > IndeterminateFormRep) ? Layout::InfinityToZeroPowerRep : UnknownType,
      
    //ZeroPowerOfNegativeInfinity 0^{−∞} — diverges to ∞, distinct from ∞^∞.
		ZeroPowerOfNegativeInfinity = Layout::IndeterminateFormRep!=0&&(Layout::ZeroPowerOfNegativeInfinityRep > IndeterminateFormRep) ? Layout::ZeroPowerOfNegativeInfinityRep : UnknownType,
//		UndefinedButWithinZeroInfinityRange = Layout::IndeterminateFormRep!=0&&(Layout::UndefinedButWithinZeroInfinityRangeRep > IndeterminateFormRep) ? Layout::UndefinedButWithinZeroInfinityRangeRep : UnknownType,
		
    //oscillatory, no limit(Given priority over later flags same as IndeterminateForm)
		//Sin(1/x) as x→0 — oscillatory, no limit if FlagState == 1(Pi)
		//Cos(1/x) as x→0 — oscillatory, no limit if FlagState == 2(E)
		//Tan(1/x) as x→0 — oscillatory, no limit if FlagState == 3(Tan)
    OscillatoryNoLimitSin = Layout::IndeterminateFormRep!=0&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) && Layout::SinRep!=0
    ? Layout::OscillatoryNoLimitRep|Layout::SinRep : UnknownType,
    OscillatoryNoLimitCos = Layout::IndeterminateFormRep!=0&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) && Layout::CosRep!=0
    ? Layout::OscillatoryNoLimitRep|Layout::CosRep : UnknownType,
    OscillatoryNoLimitTan = Layout::IndeterminateFormRep!=0&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) && Layout::TanRep!=0
    ? Layout::OscillatoryNoLimitRep|Layout::TanRep : UnknownType,
		//OscillatoryNonTrig — non-periodic oscillations not tied to sin/cos/tan, e.g. sgn(sin(1/x)).
		OscillatoryNonTrig = Layout::IndeterminateFormRep!=0&&(Layout::OscillatoryNonTrigRep > IndeterminateFormRep) ? Layout::OscillatoryNonTrigRep : UnknownType,
		//PathDependentLimit — result depends on complex-plane approach path.
		PathDependentLimit = Layout::IndeterminateFormRep!=0&&(Layout::PathDependentLimitRep > IndeterminateFormRep) ? Layout::PathDependentLimitRep : UnknownType,
		//ChaoticNoLimit — bounded but aperiodic behaviour (rare, but trap‑worthy).
    ChaoticNoLimit = Layout::IndeterminateFormRep!=0&&(Layout::ChaoticNoLimitRep > IndeterminateFormRep) ? Layout::ChaoticNoLimitRep : UnknownType,
		//PrecisionLossUndefined — internal catastrophic cancellation or overflow kills validity.
		PrecisionLossUndefined = Layout::IndeterminateFormRep!=0&&(Layout::PrecisionLossUndefinedRep > IndeterminateFormRep) ? Layout::PrecisionLossUndefinedRep : UnknownType,
		//EvenRootOfNegative — e.g., √[even]{−5} over ℝ.
		EvenRootOfNegative = Layout::IndeterminateFormRep!=0&&(Layout::EvenRootOfNegativeRep > IndeterminateFormRep) ? Layout::EvenRootOfNegativeRep : UnknownType,
		//Result is a potential complex number which is not currently supported by current class
    //log(−x) — undefined in without complex numbers, valid only with complex number support.
		UnsupportedComplexNumberResult = Layout::IndeterminateFormRep!=0&&(Layout::UnsupportedComplexNumberResultRep > IndeterminateFormRep) ? Layout::UnsupportedComplexNumberResultRep : UnknownType,
		//poles of Γ(z).
		FactorialOfNegativeNonInteger = Layout::IndeterminateFormRep!=0&&(Layout::FactorialOfNegativeNonIntegerRep > IndeterminateFormRep) ? Layout::FactorialOfNegativeNonIntegerRep : UnknownType,
		//e.g., arcsin(2) in R.
		InverseTrigOutOfRange = Layout::IndeterminateFormRep!=0&&(Layout::InverseTrigOutOfRangeRep > IndeterminateFormRep) ? Layout::InverseTrigOutOfRangeRep : UnknownType,
    //UnimplementedOperation — not mathematically undefined, just not yet coded.
		UnimplementedOperation = Layout::IndeterminateFormRep!=0&&(Layout::NaNRep > IndeterminateFormRep) ? Layout::UnimplementedOperationRep : UnknownType,
		//DomainError — generic “arg outside allowed domain” catch-all.
		DomainError = Layout::IndeterminateFormRep!=0&&(Layout::DomainErrorRep > IndeterminateFormRep) ? Layout::DomainErrorRep : UnknownType,
		//NaN = Layout::IndeterminateFormRep!=0&&(Layout::NaNRep > IndeterminateFormRep) ? Layout::NaNRep : UnknownType,
#pragma endregion  Indeterminate forms

#pragma region  --PiNum Variants--
#pragma region  Fractional representations

	  PositiveAboveSubPiNumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
	  NegativeAboveSubPiNumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
	  PositiveSubRangePiNumByDiv = ByDivRepresented<Layout>&&PiRepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::PiNumRep : UnknownType,
		NegativeSubRangePiNumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::PiNumRep : UnknownType,

		PositiveAboveSubMixedPi = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
		NegativeAboveSubMixedPi = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
		PositiveSubRangeMixedPi = MixedFracRepresented<Layout>&&PiRepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::PiNumRep : UnknownType,
		NegativeSubRangeMixedPi = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::PiNumRep : UnknownType,

#pragma endregion  Fractional representations

#pragma region  Approaching representations
	
  #pragma region  ApproachingBottom representation

    PositiveApproachingBottomPi = ApproachingBottomRepresented<Layout> && PiRepresented<Layout> ?
    Layout::ApproachingBottomRep|PositiveSign | Layout::PiNumRep : UnknownType,

    NegativeApproachingBottomPi = ApproachingBottomRepresented<Layout> && PiRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|NegativeSign | Layout::PiNumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation
	
  #pragma region  ApproachingTop representation

    PositiveApproachingTopPi = ApproachingTopRepresented<Layout> && PiRepresented<Layout>?
    Layout::ApproachingTopRep|PositiveSign | Layout::PiNumRep : UnknownType,
		
    NegativeApproachingTopPi = ApproachingTopRepresented<Layout> && PiRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|NegativeSign | Layout::PiNumRep : UnknownType,

	#pragma endregion  ApproachingTop representation

  #pragma region  ApproachingBottom representation divided by ExtraRep

    PositiveApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> ?
    Layout::ApproachingBottomRep|Layout::NumByDivRep|PositiveSign | Layout::PiNumRep : UnknownType,
		
    NegativeApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign | Layout::PiNumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation divided by ExtraRep

  #pragma region  ApproachingTop representation divided by ExtraRep

    PositiveApproachingTopByDivPi = ApproachingTopByDivRepresented<Layout> && PiRepresented<Layout>?
    Layout::ApproachingTopByDivRep|PositiveSign | Layout::PiNumRep : UnknownType,
		
    NegativeApproachingTopByDivPi = ApproachingTopByDivRepresented<Layout> && PiRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|Layout::NumByDivRep|NegativeSign | Layout::PiNumRep : UnknownType,

  #pragma endregion  ApproachingTop representation divided by ExtraRep

	#pragma region  ApproachingMidLeft

	  PositiveApproachingMidLeftPi = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiRepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign | Layout::PiNumRep : UnknownType,
	  NegativeApproachingMidLeftPi = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign | Layout::PiNumRep : UnknownType,

	  //(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
		PositiveZeroApproachingMidLeftPi = ApproachingMidRepresented<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::PiNumRep : UnknownType,
		NegativeZeroApproachingMidLeftPi = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::PiNumRep : UnknownType,

  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
  #pragma region  ApproachingMidRight

    PositiveApproachingMidRightPi = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiRepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign | Layout::PiNumRep : UnknownType,
    NegativeApproachingMidRightPi = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign | Layout::PiNumRep : UnknownType,

    //(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
    PositiveZeroApproachingMidRightPi = ApproachingMidRepresented<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::PiNumRep : UnknownType,
    NegativeZeroApproachingMidRightPi = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::PiNumRep : UnknownType,

  #pragma endregion  ApproachingMidRight

  #pragma region  ApproachingOffsets

		PositiveApproachingBottomPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::PiNumRep:UnknownType,
		NegativeApproachingBottomPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::PiNumRep:UnknownType,

    PositiveApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::PiNumRep:UnknownType,
    NegativeApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::PiNumRep:UnknownType,
		
    PositiveApproachingBottomMinusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::PiNumRep:UnknownType,
    NegativeApproachingBottomMinusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::PiNumRep:UnknownType,

    PositiveApproachingTopMinusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::PiNumRep:UnknownType,
    NegativeApproachingTopMinusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::PiNumRep:UnknownType,

  #pragma endregion  ApproachingOffsets

#pragma endregion  Approaching representations

#pragma region  PowerOf representations

			//PiNum to power of ExtraRep
			PositiveAboveSubPiPower = PowerOfRepresented<Layout>&&PiRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPower = PowerOfRepresented<Layout>&&PiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPower = PowerOfRepresented<Layout>&&PiRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPower = PowerOfRepresented<Layout>&&PiRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|NegativeSign | Layout::PiNumRep : UnknownType,

			PositiveAboveSubPiPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,
			
			PositiveAboveSubPiPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubPiPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangePiPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangePiPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign | Layout::PiNumRep : UnknownType,

#pragma endregion  PowerOf representations

	#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangePi = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&PiRepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangePi = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&PiRepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
	#pragma endregion  WithinMinMaxRange representations

	#pragma region Other math constants
	/*
			PhiNum   Pi = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiRepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   Pi = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiRepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum Pi = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiRepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   Pi = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&PiRepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   Pi = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiRepresented<Layout>? Layout::PlasticNumRep : UnknownType,
			PlasticSquareNum   Pi = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiRepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType,
	*/
	#pragma endregion Other math constants

	#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::SinRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubSinPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::SinRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeSinPi = TrigRepresented<Layout>&&PiRepresented<Layout>? Layout::SinRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeSinPi = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::SinRep|NegativeSign | Layout::PiNumRep : UnknownType,

			PositiveAboveSubCosPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::CosRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCosPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::CosRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCosPi = TrigRepresented<Layout>&&PiRepresented<Layout>? Layout::CosRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCosPi = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::CosRep|NegativeSign | Layout::PiNumRep : UnknownType,

			PositiveAboveSubTanPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::TanRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubTanPi = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::TanRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeTanPi = TrigRepresented<Layout>&&PiRepresented<Layout>? Layout::TanRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeTanPi = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::TanRep|NegativeSign | Layout::PiNumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::SinHRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubSinHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::SinHRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeSinHPi = HyperbolicRepresented<Layout>&&PiRepresented<Layout>? Layout::SinHRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeSinHPi = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::PiNumRep : UnknownType,

			PositiveAboveSubCosHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::CosHRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCosHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::CosHRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCosHPi = HyperbolicRepresented<Layout>&&PiRepresented<Layout>? Layout::CosHRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCosHPi = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::PiNumRep : UnknownType,

			PositiveAboveSubTanHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>? Layout::TanHRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubTanHPi = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::TanHRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeTanHPi = HyperbolicRepresented<Layout>&&PiRepresented<Layout>? Layout::TanHRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeTanHPi = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>? Layout::TanHRep|NegativeSign | Layout::PiNumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfPi = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubInversedOfPi = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeInversedOfPi = InversionRepresented<Layout>&&PiRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeInversedOfPi = InversionRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::PiNumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCscPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCscPi = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCscPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcCosPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcCosPi = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcCosPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcTanPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcTanPi = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcTanPi = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCscHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCscHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCscHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcCosHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcCosHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcCosHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcTanHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcTanHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcTanHPi = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfPi = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&PiRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::PiNumRep : UnknownType,
			NegativeAboveSubReciprocalOfPi = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::PiNumRep : UnknownType,
			PositiveSubRangeReciprocalOfPi = ReciprocalRepresented<Layout>&&PiRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::PiNumRep : UnknownType,
			NegativeSubRangeReciprocalOfPi = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&PiRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::PiNumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCscPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCscPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCscPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)Pi = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubSecPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeSecPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeSecPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  secant

			//CotangentPi = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubCotPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeCotPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeCotPi = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCscHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCscHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCscHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubSecHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeSecHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeSecHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubCotHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeCotHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeCotHPi = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubCscPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeCscPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeCscPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcCosPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcCosPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcCosPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcTanPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcTanPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcTanPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiNumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcCscHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcCscHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcCscHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcCosHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcCosHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcCosHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeAboveSubArcTanHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			PositiveSubRangeArcTanHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			NegativeSubRangeArcTanHPi = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiNumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc cotangent

	#pragma endregion  Experimental symbolic trig expression flags
#pragma endregion  --PiNum Variants--

#pragma region  --ENum Variants--
#pragma region  Fractional representations

	  PositiveAboveSubENumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
	  NegativeAboveSuENumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
	  PositiveSubRangeENumByDiv = ByDivRepresented<Layout>&&ERepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::ENumRep : UnknownType,
		NegativeSubRangeENumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::ENumRep : UnknownType,

		PositiveAboveSubMixedE = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
		NegativeAboveSubMixedE = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
		PositiveSubRangeMixedE = MixedFracRepresented<Layout>&&ERepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::ENumRep : UnknownType,
		NegativeSubRangeMixedE = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::ENumRep : UnknownType,

#pragma endregion  Fractional representations

#pragma region  Approaching representations
	
  #pragma region  ApproachingBottom representation

    PositiveApproachingBottomE = ApproachingBottomRepresented<Layout> && ERepresented<Layout> ?
    Layout::ApproachingBottomRep|PositiveSign | Layout::ENumRep : UnknownType,

    NegativeApproachingBottomE = ApproachingBottomRepresented<Layout> && ERepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|NegativeSign | Layout::ENumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation
	
  #pragma region  ApproachingTop representation

    PositiveApproachingTopE = ApproachingTopRepresented<Layout> && ERepresented<Layout>?
    Layout::ApproachingTopRep|PositiveSign | Layout::ENumRep : UnknownType,
		
    NegativeApproachingTopE = ApproachingTopRepresented<Layout> && ERepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|NegativeSign | Layout::ENumRep : UnknownType,

	#pragma endregion  ApproachingTop representation

  #pragma region  ApproachingBottom representation divided by ExtraRep

    PositiveApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> ?
    Layout::ApproachingBottomRep|Layout::NumByDivRep|PositiveSign | Layout::ENumRep : UnknownType,
		
    NegativeApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign | Layout::ENumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation divided by ExtraRep

  #pragma region  ApproachingTop representation divided by ExtraRep

    PositiveApproachingTopByDivE = ApproachingTopByDivRepresented<Layout> && ERepresented<Layout>?
    Layout::ApproachingTopByDivRep|PositiveSign | Layout::ENumRep : UnknownType,
		
    NegativeApproachingTopByDivE = ApproachingTopByDivRepresented<Layout> && ERepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|Layout::NumByDivRep|NegativeSign | Layout::ENumRep : UnknownType,

  #pragma endregion  ApproachingTop representation divided by ExtraRep

	#pragma region  ApproachingMidLeft

	  PositiveApproachingMidLeftE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&ERepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign | Layout::ENumRep : UnknownType,
	  NegativeApproachingMidLeftE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&ERepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign | Layout::ENumRep : UnknownType,

	  //(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
		PositiveZeroApproachingMidLeftE = ApproachingMidRepresented<Layout>&&ERepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::ENumRep : UnknownType,
		NegativeZeroApproachingMidLeftE = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&ERepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::ENumRep : UnknownType,

  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
  #pragma region  ApproachingMidRight

    PositiveApproachingMidRightE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&ERepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign | Layout::ENumRep : UnknownType,
    NegativeApproachingMidRightE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&ERepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign | Layout::ENumRep : UnknownType,

    //(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
    PositiveZeroApproachingMidRightE = ApproachingMidRepresented<Layout>&&ERepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::ENumRep : UnknownType,
    NegativeZeroApproachingMidRightE = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&ERepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::ENumRep : UnknownType,

  #pragma endregion  ApproachingMidRight

  #pragma region  ApproachingOffsets

		PositiveApproachingBottomPlusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::ENumRep:UnknownType,
		NegativeApproachingBottomPlusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::ENumRep:UnknownType,

    PositiveApproachingTopPlusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::ENumRep:UnknownType,
    NegativeApproachingTopPlusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::ENumRep:UnknownType,
		
    PositiveApproachingBottomMinusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::ENumRep:UnknownType,
    NegativeApproachingBottomMinusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::ENumRep:UnknownType,

    PositiveApproachingTopMinusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::ENumRep:UnknownType,
    NegativeApproachingTopMinusOffsetE = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::ENumRep:UnknownType,

  #pragma endregion  ApproachingOffsets

#pragma endregion  Approaching representations

#pragma region  PowerOf representations

			//PiNum to power of ExtraRep
			PositiveAboveSubEPower = PowerOfRepresented<Layout>&&ERepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPower = PowerOfRepresented<Layout>&&ERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPower = PowerOfRepresented<Layout>&&ERepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPower = PowerOfRepresented<Layout>&&ERepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|NegativeSign | Layout::ENumRep : UnknownType,

			PositiveAboveSubEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,
			
			PositiveAboveSubEPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubEPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeEPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeEPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign | Layout::ENumRep : UnknownType,

#pragma endregion  PowerOf representations

	#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangeE = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&ERepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangeE = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&ERepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
	#pragma endregion  WithinMinMaxRange representations

	#pragma region Other math constants
	/*
			PhiNum   E = Has_EnablePhi<Policy>&&Policy::EnablePhi&&ERepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   E = Has_EnablePhi<Policy>&&Policy::EnablePhi&&ERepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum E = Has_EnablePhi<Policy>&&Policy::EnablePhi&&ERepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   E = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&ERepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   E = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&ERepresented<Layout>? Layout::PlasticNumRep : UnknownType,
			PlasticSquareNum   E = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&ERepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType,
	*/
	#pragma endregion Other math constants

	#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::SinRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubSinE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::SinRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeSinE = TrigRepresented<Layout>&&ERepresented<Layout>? Layout::SinRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeSinE = TrigRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::SinRep|NegativeSign | Layout::ENumRep : UnknownType,

			PositiveAboveSubCosE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::CosRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubCosE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::CosRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeCosE = TrigRepresented<Layout>&&ERepresented<Layout>? Layout::CosRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeCosE = TrigRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::CosRep|NegativeSign | Layout::ENumRep : UnknownType,

			PositiveAboveSubTanE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::TanRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubTanE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::TanRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeTanE = TrigRepresented<Layout>&&ERepresented<Layout>? Layout::TanRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeTanE = TrigRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::TanRep|NegativeSign | Layout::ENumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::SinHRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubSinHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::SinHRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeSinHE = HyperbolicRepresented<Layout>&&ERepresented<Layout>? Layout::SinHRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeSinHE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::ENumRep : UnknownType,

			PositiveAboveSubCosHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::CosHRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubCosHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::CosHRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeCosHE = HyperbolicRepresented<Layout>&&ERepresented<Layout>? Layout::CosHRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeCosHE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::ENumRep : UnknownType,

			PositiveAboveSubTanHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>? Layout::TanHRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubTanHE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::TanHRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeTanHE = HyperbolicRepresented<Layout>&&ERepresented<Layout>? Layout::TanHRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeTanHE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>? Layout::TanHRep|NegativeSign | Layout::ENumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfE = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubInversedOfE = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeInversedOfE = InversionRepresented<Layout>&&ERepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeInversedOfE = InversionRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::ENumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubCscE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeCscE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeCscE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubArcCosE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcCosE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcCosE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubArcTanE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcTanE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcTanE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubCscHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeCscHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeCscHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubArcCosHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcCosHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcCosHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubArcTanHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcTanHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcTanHE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfE = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&ERepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::ENumRep : UnknownType,
			NegativeAboveSubReciprocalOfE = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::ENumRep : UnknownType,
			PositiveSubRangeReciprocalOfE = ReciprocalRepresented<Layout>&&ERepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::ENumRep : UnknownType,
			NegativeSubRangeReciprocalOfE = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&ERepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::ENumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubCscE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeCscE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeCscE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)E = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubSecE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeSecE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeSecE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			#pragma endregion  secant

			//CotangentE = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubCotE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeCotE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeCotE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubCscHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeCscHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeCscHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubSecHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeSecHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeSecHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubCotHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeCotHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeCotHE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubCscE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeCscE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeCscE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::ENumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubArcCosE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcCosE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcCosE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::ENumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubArcTanE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcTanE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcTanE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::ENumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubArcCscHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::ENumRep : UnknownType,
			PositiveSubRangeArcCscHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			NegativeSubRangeArcCscHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::ENumRep : UnknownType,
			NegativeAboveSubArcCosHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcCosHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcCosHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::ENumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeAboveSubArcTanHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::ENumRep : UnknownType,
			PositiveSubRangeArcTanHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			NegativeSubRangeArcTanHE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&ERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::ENumRep : UnknownType,
			#pragma region  Hyperbolic Arc cotangent

	#pragma endregion  Experimental symbolic trig expression flags
#pragma endregion  --ENum Variants--

#pragma region  --INum Variants--
#pragma region  Fractional representations

	  PositiveAboveSubINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
	  NegativeAboveSubINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
	  PositiveSubRangeINumByDiv = ByDivRepresented<Layout>&&IRepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::INumRep : UnknownType,
		NegativeSubRangeINumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::INumRep : UnknownType,

		PositiveAboveSubMixedI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
		NegativeAboveSubMixedI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
		PositiveSubRangeMixedI = MixedFracRepresented<Layout>&&IRepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::INumRep : UnknownType,
		NegativeSubRangeMixedI = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::INumRep : UnknownType,

#pragma endregion  Fractional representations

#pragma region  Approaching representations
	
  #pragma region  ApproachingBottom representation

    PositiveApproachingBottomI = ApproachingBottomRepresented<Layout> && IRepresented<Layout> ?
    Layout::ApproachingBottomRep|PositiveSign | Layout::INumRep : UnknownType,

    NegativeApproachingBottomI = ApproachingBottomRepresented<Layout> && IRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|NegativeSign | Layout::INumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation
	
  #pragma region  ApproachingTop representation

    PositiveApproachingTopI = ApproachingTopRepresented<Layout> && IRepresented<Layout>?
    Layout::ApproachingTopRep|PositiveSign | Layout::INumRep : UnknownType,
		
    NegativeApproachingTopI = ApproachingTopRepresented<Layout> && IRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|NegativeSign | Layout::INumRep : UnknownType,

	#pragma endregion  ApproachingTop representation

  #pragma region  ApproachingBottom representation divided by ExtraRep

    PositiveApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> ?
    Layout::ApproachingBottomRep|Layout::NumByDivRep|PositiveSign | Layout::INumRep : UnknownType,
		
    NegativeApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign | Layout::INumRep : UnknownType,

	#pragma endregion  ApproachingBottom representation divided by ExtraRep

  #pragma region  ApproachingTop representation divided by ExtraRep

    PositiveApproachingTopByDivI = ApproachingTopByDivRepresented<Layout> && IRepresented<Layout>?
    Layout::ApproachingTopByDivRep|PositiveSign | Layout::INumRep : UnknownType,
		
    NegativeApproachingTopByDivI = ApproachingTopByDivRepresented<Layout> && IRepresented<Layout> &&
    SignEnabled<Layout> ? Layout::ApproachingTopRep|Layout::NumByDivRep|NegativeSign | Layout::INumRep : UnknownType,

  #pragma endregion  ApproachingTop representation divided by ExtraRep

	#pragma region  ApproachingMidLeft

	  PositiveApproachingMidLeftI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&IRepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign | Layout::INumRep : UnknownType,
	  NegativeApproachingMidLeftI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&IRepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign | Layout::INumRep : UnknownType,

	  //(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
		PositiveZeroApproachingMidLeftI = ApproachingMidRepresented<Layout>&&IRepresented<Layout>? Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::INumRep : UnknownType,
		NegativeZeroApproachingMidLeftI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&IRepresented<Layout>? Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::INumRep : UnknownType,

  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
  #pragma region  ApproachingMidRight

    PositiveApproachingMidRightI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&IRepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign | Layout::INumRep : UnknownType,
    NegativeApproachingMidRightI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&IRepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign | Layout::INumRep : UnknownType,

    //(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
    PositiveZeroApproachingMidRightI = ApproachingMidRepresented<Layout>&&IRepresented<Layout>? Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::INumRep : UnknownType,
    NegativeZeroApproachingMidRightI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&IRepresented<Layout>? Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::INumRep : UnknownType,

  #pragma endregion  ApproachingMidRight

  #pragma region  ApproachingOffsets

		PositiveApproachingBottomPlusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::INumRep:UnknownType,
		NegativeApproachingBottomPlusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep |Layout::INumRep:UnknownType,

    PositiveApproachingTopPlusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::INumRep:UnknownType,
    NegativeApproachingTopPlusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep |Layout::INumRep:UnknownType,
		
    PositiveApproachingBottomMinusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::INumRep:UnknownType,
    NegativeApproachingBottomMinusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep |Layout::INumRep:UnknownType,

    PositiveApproachingTopMinusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::INumRep:UnknownType,
    NegativeApproachingTopMinusOffsetI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
    NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep |Layout::INumRep:UnknownType,

  #pragma endregion  ApproachingOffsets

#pragma endregion  Approaching representations

#pragma region  PowerOf representations

			//PiNum to power of ExtraRep
			PositiveAboveSubIPower = PowerOfRepresented<Layout>&&IRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPower = PowerOfRepresented<Layout>&&IRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPower = PowerOfRepresented<Layout>&&IRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPower = PowerOfRepresented<Layout>&&IRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|NegativeSign | Layout::INumRep : UnknownType,

			PositiveAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiNumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:ENumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiENumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiINumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:EINumExtraRep|NegativeSign | Layout::INumRep : UnknownType,
			
			PositiveAboveSubIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::ToPowerOfRep|Layout:PiEINumExtraRep|NegativeSign | Layout::INumRep : UnknownType,

#pragma endregion  PowerOf representations

	#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangeI = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&IRepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangeI = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&IRepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
	#pragma endregion  WithinMinMaxRange representations

	#pragma region Other math constants
	/*
			PhiNum   I = Has_EnablePhi<Policy>&&Policy::EnablePhi&&IRepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   I = Has_EnablePhi<Policy>&&Policy::EnablePhi&&IRepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum I = Has_EnablePhi<Policy>&&Policy::EnablePhi&&IRepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   I = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&IRepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   I = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&IRepresented<Layout>? Layout::PlasticNumRep : UnknownType,
			PlasticSquareNum   I = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&IRepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType,
	*/
	#pragma endregion Other math constants

	#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::SinRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubSinI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::SinRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeSinI = TrigRepresented<Layout>&&IRepresented<Layout>? Layout::SinRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeSinI = TrigRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::SinRep|NegativeSign | Layout::INumRep : UnknownType,

			PositiveAboveSubCosI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::CosRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubCosI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::CosRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeCosI = TrigRepresented<Layout>&&IRepresented<Layout>? Layout::CosRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeCosI = TrigRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::CosRep|NegativeSign | Layout::INumRep : UnknownType,

			PositiveAboveSubTanI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::TanRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubTanI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::TanRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeTanI = TrigRepresented<Layout>&&IRepresented<Layout>? Layout::TanRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeTanI = TrigRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::TanRep|NegativeSign | Layout::INumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::SinHRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubSinHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::SinHRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeSinHI = HyperbolicRepresented<Layout>&&IRepresented<Layout>? Layout::SinHRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeSinHI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::INumRep : UnknownType,

			PositiveAboveSubCosHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::CosHRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubCosHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::CosHRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeCosHI = HyperbolicRepresented<Layout>&&IRepresented<Layout>? Layout::CosHRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeCosHI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::INumRep : UnknownType,

			PositiveAboveSubTanHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>? Layout::TanHRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubTanHI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::TanHRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeTanHI = HyperbolicRepresented<Layout>&&IRepresented<Layout>? Layout::TanHRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeTanHI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>? Layout::TanHRep|NegativeSign | Layout::INumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubInversedOfI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeInversedOfI = InversionRepresented<Layout>&&IRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeInversedOfI = InversionRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::INumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeAboveSubCscI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			PositiveSubRangeCscI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeSubRangeCscI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::INumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::INumRep : UnknownType,
			NegativeAboveSubArcCosI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcCosI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcCosI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::INumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeAboveSubArcTanI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcTanI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcTanI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::INumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubCscHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			PositiveSubRangeCscHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeSubRangeCscHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubArcCosHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcCosHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcCosHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeAboveSubArcTanHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcTanHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcTanHI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&IRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::INumRep : UnknownType,
			NegativeAboveSubReciprocalOfI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::INumRep : UnknownType,
			PositiveSubRangeReciprocalOfI = ReciprocalRepresented<Layout>&&IRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::INumRep : UnknownType,
			NegativeSubRangeReciprocalOfI = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&IRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::INumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeAboveSubCscI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			PositiveSubRangeCscI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeSubRangeCscI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::INumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)I = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::INumRep : UnknownType,
			NegativeAboveSubSecI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::INumRep : UnknownType,
			PositiveSubRangeSecI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::INumRep : UnknownType,
			NegativeSubRangeSecI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::INumRep : UnknownType,
			#pragma endregion  secant

			//CotangentI = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeAboveSubCotI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			PositiveSubRangeCotI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeSubRangeCotI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::INumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubCscHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			PositiveSubRangeCscHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeSubRangeCscHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubSecHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeSecHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeSecHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeAboveSubCotHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeCotHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeCotHI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeAboveSubCscI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::INumRep : UnknownType,
			PositiveSubRangeCscI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::INumRep : UnknownType,
			NegativeSubRangeCscI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::INumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::INumRep : UnknownType,
			NegativeAboveSubArcCosI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcCosI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcCosI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::INumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeAboveSubArcTanI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcTanI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcTanI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::INumRep : UnknownType,
			#pragma endregion  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubArcCscHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::INumRep : UnknownType,
			PositiveSubRangeArcCscHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			NegativeSubRangeArcCscHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::INumRep : UnknownType,
			NegativeAboveSubArcCosHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcCosHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcCosHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeAboveSubArcTanHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::INumRep : UnknownType,
			PositiveSubRangeArcTanHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			NegativeSubRangeArcTanHI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&IRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::INumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc cotangent

	#pragma endregion  Experimental symbolic trig expression flags
#pragma endregion  --INum Variants--

#pragma region  --PiENum Variants--
  #pragma region  Fractional representations

			PositiveAboveSubPiINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiINumByDiv = ByDivRepresented<Layout>&&PiERepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiINumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::PiENumRep : UnknownType,

			PositiveAboveSubMixedPiE = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubMixedPiE = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeMixedPiE = MixedFracRepresented<Layout>&&PiERepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeMixedPiE = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::PiENumRep : UnknownType,

  #pragma endregion  Fractional representations

  #pragma region  Approaching representations

		  PositiveApproachingBottomPiE = ApproachingRepresented<Layout>&&PiERepresented<Layout>? 
      Layout::ApproachingBottomRep|PositiveSign| Layout::PiENumRep : UnknownType,
	  	NegativeApproachingBottomPiE = ApproachingRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingBottomRep|NegativeSign| Layout::PiENumRep : UnknownType,

		  PositiveApproachingTopPiE = ApproachingTopRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign| Layout::PiENumRep : UnknownType,
		  NegativeApproachingTopPiE = ApproachingTopRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingTopRep|NegativeSign| Layout::PiENumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

      PositiveApproachingBottomByDivPiE = ApproachingBottomByDivRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingBottomByDivRep|PositiveSign| Layout::PiENumRep : UnknownType,
		
      NegativeApproachingBottomByDivPiE = ApproachingBottomByDivRepresented<Layout> && SignEnabled<Layout>
      &&PiERepresented<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign| Layout::PiENumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

    #pragma region  ApproachingTop representation divided by ExtraRep

      PositiveApproachingTopPiE = ApproachingRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeApproachingTopPiE = ApproachingRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingTopRep|NegativeSign | Layout::PiENumRep : UnknownType,

	  #pragma endregion  ApproachingTop representation divided by ExtraRep

	  #pragma region  ApproachingMidLeft

			PositiveApproachingMidLeftPiE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiERepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeApproachingMidLeftPiE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign | Layout::PiENumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
			PositiveZeroApproachingMidLeftPiE = ApproachingMidRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::PiENumRep : UnknownType,
			NegativeZeroApproachingMidLeftPiE = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::PiENumRep : UnknownType,

	  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
	#pragma region  ApproachingMidRight

			PositiveApproachingMidRightPiE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiERepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeApproachingMidRightPiE = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign | Layout::PiENumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
			PositiveZeroApproachingMidRightPiE = ApproachingMidRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::PiENumRep : UnknownType,
			NegativeZeroApproachingMidRightPiE = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::PiENumRep : UnknownType,

	#pragma endregion  ApproachingMidRight

	#pragma region  ApproachingOffsets

		  PositiveApproachingBottomPlusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiENumRep:UnknownType,
		  NegativeApproachingBottomPlusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiENumRep:UnknownType,

      PositiveApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiENumRep:UnknownType,
      NegativeApproachingTopPlusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiENumRep:UnknownType,
		
      PositiveApproachingBottomMinusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiENumRep:UnknownType,
      NegativeApproachingBottomMinusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiENumRep:UnknownType,

      PositiveApproachingTopMinusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiENumRep:UnknownType,
      NegativeApproachingTopMinusOffsetPiE = ApproachingOffsetsRepresented<Layout>&&PiERepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiENumRep:UnknownType,

		#pragma endregion  ApproachingOffsets

  #pragma endregion  Approaching representations

  #pragma region  PowerOf representations

	    PositiveAboveSubPiEPower = PowerOfRepresented<Layout>&&PiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
	    NegativeAboveSubPiEPower = PowerOfRepresented<Layout>&&PiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
	    PositiveSubRangePiEPower = PowerOfRepresented<Layout>&&PiERepresented<Layout>&&PiERepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign | Layout::PiENumRep : UnknownType,
		  NegativeSubRangePiEPower = PowerOfRepresented<Layout>&&PiERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign | Layout::PiENumRep : UnknownType,

		  PositiveAboveSubPiEPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
		  NegativeAboveSubPiEPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfPiRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfERep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfERep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfERep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::PowerOfERep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiERep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiERep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiERep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiERep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfEIRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfEIRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiERepresented<Layout>? Layout::PowerOfEIRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfEIRep|NegativeSign | Layout::PiENumRep : UnknownType,
			
			PositiveAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubPiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangePiEPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign | Layout::PiENumRep : UnknownType,

#pragma endregion  PowerOf representations

#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangePiE = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&PiERepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangePiE = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&PiERepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
#pragma endregion  WithinMinMaxRange representations

#pragma region Other math constants
	/*
			PhiNum   PiE = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiERepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   PiE = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiERepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum PiE = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiERepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   PiE = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&PiERepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   PiE = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiERepresented<Layout>? Layout::PlasticNumRep : UnknownType
			PlasticSquareNum   PiE = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiERepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType
	*/
#pragma region Other math constants

#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::SinRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubSinPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::SinRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeSinPiE = TrigRepresented<Layout>&&PiERepresented<Layout>?
      Layout::SinRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeSinPiE = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::SinRep|NegativeSign | Layout::PiENumRep : UnknownType,

			PositiveAboveSubCosPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::CosRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCosPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::CosRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCosPiE = TrigRepresented<Layout>&&PiERepresented<Layout>?
      Layout::CosRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCosPiE = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::CosRep|NegativeSign | Layout::PiENumRep : UnknownType,

			PositiveAboveSubTanPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubTanPiE = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeTanPiE = TrigRepresented<Layout>&&PiERepresented<Layout>?
      Layout::TanRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeTanPiE = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanRep|NegativeSign | Layout::PiENumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::SinHRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubSinHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::SinHRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeSinHPiE = HyperbolicRepresented<Layout>&&PiERepresented<Layout>?
      Layout::SinHRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeSinHPiE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::PiENumRep : UnknownType,

			PositiveAboveSubCosHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::CosHRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCosHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::CosHRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCosHPiE = HyperbolicRepresented<Layout>&&PiERepresented<Layout>?
      Layout::CosHRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCosHPiE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::PiENumRep : UnknownType,

			PositiveAboveSubTanHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanHRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubTanHPiE = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanHRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeTanHPiE = HyperbolicRepresented<Layout>&&PiERepresented<Layout>?
      Layout::TanHRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeTanHPiE = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::TanHRep|NegativeSign | Layout::PiENumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfPiE = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubInversedOfPiE = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeInversedOfPiE = InversionRepresented<Layout>&&PiERepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeInversedOfPiE = InversionRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::PiENumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCscPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCscPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCscPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcCosPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcCosPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcCosPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcTanPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcTanPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcTanPiE = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCscHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCscHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCscHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcCosHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcCosHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcCosHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcTanHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcTanHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcTanHPiE = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfPiE = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::PiENumRep : UnknownType,
			NegativeAboveSubReciprocalOfPiE = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::PiENumRep : UnknownType,
			PositiveSubRangeReciprocalOfPiE = ReciprocalRepresented<Layout>&&PiERepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::PiENumRep : UnknownType,
			NegativeSubRangeReciprocalOfPiE = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&PiERepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::PiENumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCscPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCscPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCscPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)PiE = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubSecPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeSecPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeSecPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  secant

			//CotangentPiE = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubCotPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeCotPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeCotPiE = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCscHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCscHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCscHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubSecHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeSecHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeSecHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubCotHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeCotHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeCotHPiE = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubCscPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeCscPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeCscPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcCosPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcCosPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcCosPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcTanPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcTanPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcTanPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiENumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcCscHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcCscHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcCscHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcCosHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcCosHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcCosHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiENumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeAboveSubArcTanHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			PositiveSubRangeArcTanHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			NegativeSubRangeArcTanHPiE = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiERepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiENumRep : UnknownType,
			#pragma region  Hyperbolic Arc cotangent

	#pragma region  Experimental symbolic trig expression flags
#pragma endregion  --PiENum Variants--

#pragma region  --PiINum Variants--
  #pragma region  Fractional representations

			PositiveAboveSubPiINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiINumByDiv = ByDivRepresented<Layout>&&PiIRepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiINumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::PiINumRep : UnknownType,

			PositiveAboveSubMixedPiI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubMixedPiI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeMixedPiI = MixedFracRepresented<Layout>&&PiIRepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeMixedPiI = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::PiINumRep : UnknownType,

  #pragma endregion  Fractional representations

  #pragma region  Approaching representations

		  PositiveApproachingBottomPiI = ApproachingRepresented<Layout>&&PiIRepresented<Layout>? 
      Layout::ApproachingBottomRep|PositiveSign| Layout::PiINumRep : UnknownType,
	  	NegativeApproachingBottomPiI = ApproachingRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingBottomRep|NegativeSign| Layout::PiINumRep : UnknownType,

		  PositiveApproachingTopPiI = ApproachingTopRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign| Layout::PiINumRep : UnknownType,
		  NegativeApproachingTopPiI = ApproachingTopRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingTopRep|NegativeSign| Layout::PiINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

      PositiveApproachingBottomByDivPiI = ApproachingBottomByDivRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingBottomByDivRep|PositiveSign| Layout::PiINumRep : UnknownType,
		
      NegativeApproachingBottomByDivPiI = ApproachingBottomByDivRepresented<Layout> && SignEnabled<Layout>
      &&PiIRepresented<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign| Layout::PiINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

    #pragma region  ApproachingTop representation divided by ExtraRep

      PositiveApproachingTopPiI = ApproachingRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeApproachingTopPiI = ApproachingRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingTopRep|NegativeSign | Layout::PiINumRep : UnknownType,

	  #pragma endregion  ApproachingTop representation divided by ExtraRep

	  #pragma region  ApproachingMidLeft

			PositiveApproachingMidLeftPiI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeApproachingMidLeftPiI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign | Layout::PiINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
			PositiveZeroApproachingMidLeftPiI = ApproachingMidRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::PiINumRep : UnknownType,
			NegativeZeroApproachingMidLeftPiI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::PiINumRep : UnknownType,

	  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
	#pragma region  ApproachingMidRight

			PositiveApproachingMidRightPiI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeApproachingMidRightPiI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign | Layout::PiINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
			PositiveZeroApproachingMidRightPiI = ApproachingMidRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::PiINumRep : UnknownType,
			NegativeZeroApproachingMidRightPiI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::PiINumRep : UnknownType,

	#pragma endregion  ApproachingMidRight

	#pragma region  ApproachingOffsets

		  PositiveApproachingBottomPlusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiINumRep:UnknownType,
		  NegativeApproachingBottomPlusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiINumRep:UnknownType,

      PositiveApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiINumRep:UnknownType,
      NegativeApproachingTopPlusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiINumRep:UnknownType,
		
      PositiveApproachingBottomMinusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiINumRep:UnknownType,
      NegativeApproachingBottomMinusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiINumRep:UnknownType,

      PositiveApproachingTopMinusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiINumRep:UnknownType,
      NegativeApproachingTopMinusOffsetPiI = ApproachingOffsetsRepresented<Layout>&&PiIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiINumRep:UnknownType,

		#pragma endregion  ApproachingOffsets

  #pragma endregion  Approaching representations

  #pragma region  PowerOf representations

	    PositiveAboveSubPiIPower = PowerOfRepresented<Layout>&&PiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
	    NegativeAboveSubPiIPower = PowerOfRepresented<Layout>&&PiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
	    PositiveSubRangePiIPower = PowerOfRepresented<Layout>&&PiIRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign | Layout::PiINumRep : UnknownType,
		  NegativeSubRangePiIPower = PowerOfRepresented<Layout>&&PiIRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign | Layout::PiINumRep : UnknownType,

		  PositiveAboveSubPiIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
		  NegativeAboveSubPiIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfPiRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&PiIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign | Layout::PiINumRep : UnknownType,
			
			PositiveAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubPiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangePiIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign | Layout::PiINumRep : UnknownType,

#pragma endregion  PowerOf representations

#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangePiI = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&PiIRepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangePiI = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&PiIRepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
#pragma endregion  WithinMinMaxRange representations

#pragma region Other math constants
	/*
			PhiNum   PiI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiIRepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   PiI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiIRepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum PiI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiIRepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   PiI = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&PiIRepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   PiI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiIRepresented<Layout>? Layout::PlasticNumRep : UnknownType
			PlasticSquareNum   PiI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiIRepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType
	*/
#pragma region Other math constants

#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::SinRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubSinPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::SinRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeSinPiI = TrigRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::SinRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeSinPiI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::SinRep|NegativeSign | Layout::PiINumRep : UnknownType,

			PositiveAboveSubCosPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::CosRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCosPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::CosRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCosPiI = TrigRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::CosRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCosPiI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::CosRep|NegativeSign | Layout::PiINumRep : UnknownType,

			PositiveAboveSubTanPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubTanPiI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeTanPiI = TrigRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::TanRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeTanPiI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanRep|NegativeSign | Layout::PiINumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::SinHRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubSinHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::SinHRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeSinHPiI = HyperbolicRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::SinHRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeSinHPiI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::PiINumRep : UnknownType,

			PositiveAboveSubCosHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::CosHRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCosHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::CosHRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCosHPiI = HyperbolicRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::CosHRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCosHPiI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::PiINumRep : UnknownType,

			PositiveAboveSubTanHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanHRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubTanHPiI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanHRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeTanHPiI = HyperbolicRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::TanHRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeTanHPiI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::TanHRep|NegativeSign | Layout::PiINumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfPiI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubInversedOfPiI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeInversedOfPiI = InversionRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeInversedOfPiI = InversionRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::PiINumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCscPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCscPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCscPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcCosPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcCosPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcCosPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcTanPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcTanPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcTanPiI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCscHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCscHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCscHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcCosHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcCosHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcCosHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcTanHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcTanHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcTanHPiI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfPiI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::PiINumRep : UnknownType,
			NegativeAboveSubReciprocalOfPiI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::PiINumRep : UnknownType,
			PositiveSubRangeReciprocalOfPiI = ReciprocalRepresented<Layout>&&PiIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::PiINumRep : UnknownType,
			NegativeSubRangeReciprocalOfPiI = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&PiIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::PiINumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCscPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCscPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCscPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)PiI = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubSecPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeSecPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeSecPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  secant

			//CotangentPiI = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubCotPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeCotPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeCotPiI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCscHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCscHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCscHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubSecHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeSecHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeSecHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubCotHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeCotHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeCotHPiI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubCscPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeCscPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeCscPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcCosPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcCosPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcCosPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcTanPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcTanPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcTanPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiINumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcCscHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcCscHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcCscHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcCosHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcCosHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcCosHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiINumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeAboveSubArcTanHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			PositiveSubRangeArcTanHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			NegativeSubRangeArcTanHPiI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiINumRep : UnknownType,
			#pragma region  Hyperbolic Arc cotangent

	#pragma region  Experimental symbolic trig expression flags
#pragma endregion  --PiINum Variants--

#pragma region  --EINum Variants--
  #pragma region  Fractional representations

			PositiveAboveSubEINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubEINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeEINumByDiv = ByDivRepresented<Layout>&&EIRepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeEINumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::EINumRep : UnknownType,

			PositiveAboveSubMixedEI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubMixedEI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeMixedEI = MixedFracRepresented<Layout>&&EIRepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeMixedEI = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::EINumRep : UnknownType,

  #pragma endregion  Fractional representations

  #pragma region  Approaching representations

		  PositiveApproachingBottomEI = ApproachingRepresented<Layout>&&EIRepresented<Layout>? 
      Layout::ApproachingBottomRep|PositiveSign| Layout::EINumRep : UnknownType,
	  	NegativeApproachingBottomEI = ApproachingRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingBottomRep|NegativeSign| Layout::EINumRep : UnknownType,

		  PositiveApproachingTopEI = ApproachingTopRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign| Layout::EINumRep : UnknownType,
		  NegativeApproachingTopEI = ApproachingTopRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingTopRep|NegativeSign| Layout::EINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

      PositiveApproachingBottomByDivEI = ApproachingBottomByDivRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingBottomByDivRep|PositiveSign| Layout::EINumRep : UnknownType,
		
      NegativeApproachingBottomByDivEI = ApproachingBottomByDivRepresented<Layout> && SignEnabled<Layout>
      &&EIRepresented<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign| Layout::EINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

    #pragma region  ApproachingTop representation divided by ExtraRep

      PositiveApproachingTopEI = ApproachingRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeApproachingTopEI = ApproachingRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingTopRep|NegativeSign | Layout::EINumRep : UnknownType,

	  #pragma endregion  ApproachingTop representation divided by ExtraRep

	  #pragma region  ApproachingMidLeft

			PositiveApproachingMidLeftEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&EIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeApproachingMidLeftEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign | Layout::EINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
			PositiveZeroApproachingMidLeftEI = ApproachingMidRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::EINumRep : UnknownType,
			NegativeZeroApproachingMidLeftEI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::EINumRep : UnknownType,

	  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
	#pragma region  ApproachingMidRight

			PositiveApproachingMidRightEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&EIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeApproachingMidRightEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign | Layout::EINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
			PositiveZeroApproachingMidRightEI = ApproachingMidRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::EINumRep : UnknownType,
			NegativeZeroApproachingMidRightEI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::EINumRep : UnknownType,

	#pragma endregion  ApproachingMidRight

	#pragma region  ApproachingOffsets

		  PositiveApproachingBottomPlusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::EINumRep:UnknownType,
		  NegativeApproachingBottomPlusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::EINumRep:UnknownType,

      PositiveApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::EINumRep:UnknownType,
      NegativeApproachingTopPlusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::EINumRep:UnknownType,
		
      PositiveApproachingBottomMinusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::EINumRep:UnknownType,
      NegativeApproachingBottomMinusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::EINumRep:UnknownType,

      PositiveApproachingTopMinusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::EINumRep:UnknownType,
      NegativeApproachingTopMinusOffsetEI = ApproachingOffsetsRepresented<Layout>&&EIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::EINumRep:UnknownType,

		#pragma endregion  ApproachingOffsets

  #pragma endregion  Approaching representations

  #pragma region  PowerOf representations

	    PositiveAboveSubPiEIPower = PowerOfRepresented<Layout>&&EIRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
	    NegativeAboveSubPiEIPower = PowerOfRepresented<Layout>&&EIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
	    PositiveSubRangePiEIPower = PowerOfRepresented<Layout>&&EIRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign | Layout::EINumRep : UnknownType,
		  NegativeSubRangePiEIPower = PowerOfRepresented<Layout>&&EIRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign | Layout::EINumRep : UnknownType,

		  PositiveAboveSubPiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
		  NegativeAboveSubPiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfPiRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&EIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign | Layout::EINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign | Layout::EINumRep : UnknownType,

#pragma endregion  PowerOf representations

#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangeEI = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&EIRepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangeEI = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&EIRepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
#pragma endregion  WithinMinMaxRange representations

#pragma region Other math constants
	/*
			PhiNum   EI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&EIRepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   EI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&EIRepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum EI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&EIRepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   EI = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&EIRepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   EI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&EIRepresented<Layout>? Layout::PlasticNumRep : UnknownType
			PlasticSquareNum   EI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&EIRepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType
	*/
#pragma region Other math constants

#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::SinRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubSinEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::SinRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeSinEI = TrigRepresented<Layout>&&EIRepresented<Layout>?
      Layout::SinRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeSinEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::SinRep|NegativeSign | Layout::EINumRep : UnknownType,

			PositiveAboveSubCosEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::CosRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubCosEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::CosRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeCosEI = TrigRepresented<Layout>&&EIRepresented<Layout>?
      Layout::CosRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeCosEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::CosRep|NegativeSign | Layout::EINumRep : UnknownType,

			PositiveAboveSubTanEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubTanEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeTanEI = TrigRepresented<Layout>&&EIRepresented<Layout>?
      Layout::TanRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeTanEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanRep|NegativeSign | Layout::EINumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::SinHRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubSinHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::SinHRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeSinHEI = HyperbolicRepresented<Layout>&&EIRepresented<Layout>?
      Layout::SinHRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeSinHEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::EINumRep : UnknownType,

			PositiveAboveSubCosHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::CosHRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubCosHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::CosHRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeCosHEI = HyperbolicRepresented<Layout>&&EIRepresented<Layout>?
      Layout::CosHRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeCosHEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::EINumRep : UnknownType,

			PositiveAboveSubTanHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanHRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubTanHEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanHRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeTanHEI = HyperbolicRepresented<Layout>&&EIRepresented<Layout>?
      Layout::TanHRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeTanHEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::TanHRep|NegativeSign | Layout::EINumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfEI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubInversedOfEI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeInversedOfEI = InversionRepresented<Layout>&&EIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeInversedOfEI = InversionRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::EINumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubCscEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeCscEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeCscEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubArcCosEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcCosEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcCosEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubArcTanEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcTanEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcTanEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubCscHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeCscHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeCscHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubArcCosHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcCosHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcCosHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubArcTanHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcTanHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcTanHEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfEI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::EINumRep : UnknownType,
			NegativeAboveSubReciprocalOfEI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::EINumRep : UnknownType,
			PositiveSubRangeReciprocalOfEI = ReciprocalRepresented<Layout>&&EIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::EINumRep : UnknownType,
			NegativeSubRangeReciprocalOfEI = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&EIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::EINumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubCscEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeCscEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeCscEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)EI = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubSecEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeSecEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeSecEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			#pragma endregion  secant

			//CotangentEI = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubCotEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeCotEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeCotEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubCscHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeCscHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeCscHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubSecHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeSecHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeSecHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubCotHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeCotHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeCotHEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubCscEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeCscEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeCscEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::EINumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubArcCosEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcCosEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcCosEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::EINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubArcTanEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcTanEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcTanEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::EINumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubArcCscHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::EINumRep : UnknownType,
			PositiveSubRangeArcCscHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			NegativeSubRangeArcCscHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::EINumRep : UnknownType,
			NegativeAboveSubArcCosHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcCosHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcCosHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::EINumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeAboveSubArcTanHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::EINumRep : UnknownType,
			PositiveSubRangeArcTanHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			NegativeSubRangeArcTanHEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&EIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::EINumRep : UnknownType,
			#pragma region  Hyperbolic Arc cotangent

	#pragma region  Experimental symbolic trig expression flags
#pragma endregion  --EINum Variants--

#pragma region  --PiEINum Variants--
  #pragma region  Fractional representations

			PositiveAboveSubPiEINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::NumByDivRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEINumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::NumByDivRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEINumByDiv = ByDivRepresented<Layout>&&PiEIRepresented<Layout>? Layout::NumByDivRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEINumByDiv = ByDivRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::NumByDivRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			PositiveAboveSubMixedPiEI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::MixedFracRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubMixedPiEI = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::MixedFracRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeMixedPiEI = MixedFracRepresented<Layout>&&PiEIRepresented<Layout>? Layout::MixedFracRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeMixedPiEI = MixedFracRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::MixedFracRep|NegativeSign | Layout::PiEINumRep : UnknownType,

  #pragma endregion  Fractional representations

  #pragma region  Approaching representations

		  PositiveApproachingBottomPiEI = ApproachingRepresented<Layout>&&PiEIRepresented<Layout>? 
      Layout::ApproachingBottomRep|PositiveSign| Layout::PiEINumRep : UnknownType,
	  	NegativeApproachingBottomPiEI = ApproachingRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingBottomRep|NegativeSign| Layout::PiEINumRep : UnknownType,

		  PositiveApproachingTopPiEI = ApproachingTopRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign| Layout::PiEINumRep : UnknownType,
		  NegativeApproachingTopPiEI = ApproachingTopRepresented<Layout>&&SignEnabled<Layout> ?
      Layout::ApproachingTopRep|NegativeSign| Layout::PiEINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

      PositiveApproachingBottomByDivPiEI = ApproachingBottomByDivRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingBottomByDivRep|PositiveSign| Layout::PiEINumRep : UnknownType,
		
      NegativeApproachingBottomByDivPiEI = ApproachingBottomByDivRepresented<Layout> && SignEnabled<Layout>
      &&PiEIRepresented<Layout> ? Layout::ApproachingBottomRep|Layout::NumByDivRep|NegativeSign| Layout::PiEINumRep : UnknownType,

    #pragma region  ApproachingBottom representation divided by ExtraRep

    #pragma region  ApproachingTop representation divided by ExtraRep

      PositiveApproachingTopPiEI = ApproachingRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingTopRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeApproachingTopPiEI = ApproachingRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingTopRep|NegativeSign | Layout::PiEINumRep : UnknownType,

	  #pragma endregion  ApproachingTop representation divided by ExtraRep

	  #pragma region  ApproachingMidLeft

			PositiveApproachingMidLeftPiEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeApproachingMidLeftPiEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))-0.0...01)
			PositiveZeroApproachingMidLeftPiEI = ApproachingMidRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|PositiveSign|ZeroSentinal | Layout::PiEINumRep : UnknownType,
			NegativeZeroApproachingMidLeftPiEI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidLeftRep|NegativeSign|ZeroSentinal | Layout::PiEINumRep : UnknownType,

	  #pragma endregion  ApproachingMidLeft

	// Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
	#pragma region  ApproachingMidRight

			PositiveApproachingMidRightPiEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeApproachingMidRightPiEI = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> && SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			//(0+(DecimalOverflow/(Raw as unsigned))+0.0...01)
			PositiveZeroApproachingMidRightPiEI = ApproachingMidRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidRightRep|PositiveSign|ZeroSentinal | Layout::PiEINumRep : UnknownType,
			NegativeZeroApproachingMidRightPiEI = ApproachingMidRepresented<Layout> && SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ApproachingMidRightRep|NegativeSign|ZeroSentinal | Layout::PiEINumRep : UnknownType,

	#pragma endregion  ApproachingMidRight

	#pragma region  ApproachingOffsets

		  PositiveApproachingBottomPlusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiEINumRep:UnknownType,
		  NegativeApproachingBottomPlusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::PlusOffsetRep | Layout::PiEINumRep:UnknownType,

      PositiveApproachingTopPlusOffsetPi = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiEINumRep:UnknownType,
      NegativeApproachingTopPlusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::PlusOffsetRep | Layout::PiEINumRep:UnknownType,
		
      PositiveApproachingBottomMinusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      PositiveSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiEINumRep:UnknownType,
      NegativeApproachingBottomMinusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      NegativeSign|Layout::ApproachingBottomRep|Layout::MinusOffsetRep | Layout::PiEINumRep:UnknownType,

      PositiveApproachingTopMinusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      PositiveSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiEINumRep:UnknownType,
      NegativeApproachingTopMinusOffsetPiEI = ApproachingOffsetsRepresented<Layout>&&PiEIRepresented<Layout>?
      NegativeSign|Layout::ApproachingTopRep|Layout::MinusOffsetRep | Layout::PiEINumRep:UnknownType,

		#pragma endregion  ApproachingOffsets

  #pragma endregion  Approaching representations

  #pragma region  PowerOf representations

	    PositiveAboveSubPiEIPower = PowerOfRepresented<Layout>&&PiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
	    NegativeAboveSubPiEIPower = PowerOfRepresented<Layout>&&PiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
	    PositiveSubRangePiEIPower = PowerOfRepresented<Layout>&&PiEIRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ToPowerOfRep|PositiveSign | Layout::PiEINumRep : UnknownType,
		  NegativeSubRangePiEIPower = PowerOfRepresented<Layout>&&PiEIRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ToPowerOfRep|NegativeSign | Layout::PiEINumRep : UnknownType,

		  PositiveAboveSubPiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ToPowerOfRep|Layout::PiNumExtraRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
		  NegativeAboveSubPiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfPiRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfPiRep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfERep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::PowerOfERep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiERep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiERep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>? Layout::PowerOfIRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfIRep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfEIRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfEIRep|NegativeSign | Layout::PiEINumRep : UnknownType,
			
			PositiveAboveSubPiEIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubPiEIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangePiEIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiEIRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangePiEIPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::PowerOfPiEIRep|NegativeSign | Layout::PiEINumRep : UnknownType,

#pragma endregion  PowerOf representations

#pragma region  WithinMinMaxRange representations
	/*
			//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			WithinMinMaxRangePiEI = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange&&PiEIRepresented<Layout>? Layout::WithinMinMaxRangeRep : UnknownType,
			//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
			NotWithinMinMaxRangePiEI = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange&&PiEIRepresented<Layout>? Layout::NotWithinMinMaxRangeRep : UnknownType,
	*/
#pragma endregion  WithinMinMaxRange representations

#pragma region Other math constants
	/*
			PhiNum   PiEI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiEIRepresented<Layout>? Layout::PhiNumRep : UnknownType,
			//√5
			SqrtXNum   PiEI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiEIRepresented<Layout>? Layout::SqrtXNumRep : UnknownType,
			PhiConjugateNum PiEI = Has_EnablePhi<Policy>&&Policy::EnablePhi&&PiEIRepresented<Layout>? Layout::DividedByPhiNumRep : UnknownType,
			//ln(2)
			LnXNum   PiEI = Has_EnableLnXNum<Policy>&&Policy::EnableLnX&&PiEIRepresented<Layout>? Layout::LnXRep : UnknownType
			PlasticNum   PiEI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiEIRepresented<Layout>? Layout::PlasticNumRep : UnknownType
			PlasticSquareNum   PiEI = Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum&&PiEIRepresented<Layout>? Layout::PlasticSquareNumRep : UnknownType
	*/
#pragma region Other math constants

#pragma region  Experimental symbolic trig expression flags
			// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
			#pragma region  Circular symbolic trig expression flags

			PositiveAboveSubSinPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::SinRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubSinPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::SinRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeSinPiEI = TrigRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::SinRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeSinPiEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::SinRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			PositiveAboveSubCosPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::CosRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCosPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::CosRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCosPiEI = TrigRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::CosRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCosPiEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::CosRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			PositiveAboveSubTanPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubTanPiEI = TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeTanPiEI = TrigRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::TanRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeTanPiEI = TrigRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			#pragma endregion  Circular symbolic trig expression flags

			#pragma region  Hyperbolic symbolic expression flags

			PositiveAboveSubSinHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::SinHRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubSinHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::SinHRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeSinHPiEI = HyperbolicRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::SinHRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeSinHPiEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::SinHRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			PositiveAboveSubCosHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::CosHRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCosHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::CosHRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCosHPiEI = HyperbolicRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::CosHRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCosHPiEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>? Layout::CosHRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			PositiveAboveSubTanHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanHRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubTanHPiEI = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanHRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeTanHPiEI = HyperbolicRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::TanHRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeTanHPiEI = HyperbolicRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::TanHRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			#pragma endregion  Hyperbolic symbolic expression flags

			#pragma region  Inversed expressions

			//Inversed Circular symbolic expression flag
			PositiveAboveSubInversedOfPiEI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubInversedOfPiEI = InversionRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeInversedOfPiEI = InversionRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::InversedOfRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeInversedOfPiEI = InversionRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::InversedOfRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			#pragma region  ArcSin
			PositiveAboveSubCscPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCscPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCscPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCscPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  ArcSin

			#pragma region  ArcCos
			PositiveAboveSubArcCosPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcCosPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcCosPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcCosPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  ArcTan
			PositiveAboveSubArcTanPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcTanPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcTanPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcTanPiEI = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			#pragma region  ArcTan

			#pragma region  Hyperbolic ArcSin
			PositiveAboveSubCscHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCscHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCscHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCscHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic ArcCos
			PositiveAboveSubArcCosHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcCosHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcCosHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcCosHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcCos

			#pragma region  Hyperbolic ArcTan
			PositiveAboveSubArcTanHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcTanHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcTanHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcTanHPiEI = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			#pragma region  Hyperbolic ArcTan

			#pragma endregion  Inversed expressions

			#pragma region  Reciprocal expressions

			//Reciprocal expression flag 1/OtherExpression/Value
			PositiveAboveSubReciprocalOfPiEI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubReciprocalOfPiEI = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeReciprocalOfPiEI = ReciprocalRepresented<Layout>&&PiEIRepresented<Layout>?
      Layout::ReciprocalOfRep|PositiveSign | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeReciprocalOfPiEI = ReciprocalRepresented<Layout>&&SignEnabled<Layout>&&PiEIRepresented<Layout>?
      Layout::ReciprocalOfRep|NegativeSign | Layout::PiEINumRep : UnknownType,

			//cosecant(x) == 1/sin(x)
			#pragma region  cosecant
			PositiveAboveSubCscPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCscPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCscPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCscPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  cosecant

			//secant(x)PiEI = 1/cos(x)
			#pragma region  secant
			PositiveAboveSubSecPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubSecPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeSecPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeSecPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  secant

			//CotangentPiEI = 1/tan(x)
			#pragma region  Cotangent
			PositiveAboveSubCotPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCotPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCotPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCotPiEI = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			#pragma region  Cotangent

			#pragma region  Hyperbolic cosecant
			PositiveAboveSubCscHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCscHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCscHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCscHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic cosecant

			#pragma region  Hyperbolic secant
			PositiveAboveSubSecHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubSecHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeSecHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeSecHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic secant

			#pragma region  Hyperbolic Cotangent
			PositiveAboveSubCotHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCotHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCotHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|PositiveSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCotHPiEI = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|NegativeSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			#pragma region  Hyperbolic Cotangent

			#pragma endregion  Reciprocal expressions

			#pragma region  Arc cosecant
			PositiveAboveSubCscPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubCscPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeCscPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeCscPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  Arc cosecant

			#pragma region  Arc secant
			PositiveAboveSubArcCosPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcCosPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcCosPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcCosPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  ArcCos

			#pragma region  Arc cotangent
			PositiveAboveSubArcTanPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcTanPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcTanPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcTanPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanRep| Layout::PiEINumRep : UnknownType,
			#pragma region  Arc cotangent

			#pragma region  Hyperbolic Arc cosecant
			PositiveAboveSubArcCscHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcCscHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcCscHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcCscHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout> &&
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::SinHRep | Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic ArcSin

			#pragma region  Hyperbolic Arc secant
			PositiveAboveSubArcCosHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::CosHRep | Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcCosHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcCosHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcCosHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::CosHRep| Layout::PiEINumRep : UnknownType,
			#pragma endregion  Hyperbolic Arc secant

			#pragma region  Hyperbolic Arc cotangent
			PositiveAboveSubArcTanHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeAboveSubArcTanHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign| AboveSub | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			PositiveSubRangeArcTanHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|PositiveSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			NegativeSubRangeArcTanHPiEI = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignEnabled<Layout>
			&&PiEIRepresented<Layout>? Layout::ReciprocalOfRep|Layout::InversedOfRep|NegativeSign | Layout::TanHRep| Layout::PiEINumRep : UnknownType,
			#pragma region  Hyperbolic Arc cotangent

	#pragma region  Experimental symbolic trig expression flags
#pragma endregion  --PiEINum Variants--

  },
	
}