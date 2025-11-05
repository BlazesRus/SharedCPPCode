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
  enum class RepTypeV3 : RepUnderlying<Layout> {
		// Unknown category used to remove unused enum features
    // No real category will be assigned to this slot.
		UnknownType  = std::numeric_limits<RepUnderlying<Layout>>::max(),

	  //At or Above one
    AboveSub = Policy::SignedExpMode ? Layout::ExpSignRep : UnknownType,
	
#pragma region NormalType

		AboveSubNormalType = Policy::SignedExpMode ? AboveSub : UnknownType,
		SubRangeNormalType = 0,

#pragma endregion NormalType

#pragma endregion Fractional representations

	  AboveSubNumByDiv = ByDivRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::NumByDivRep| AboveSub : UnknownType,
	  SubRangeNumByDiv = ByDivRepresented<Layout>? Layout::NumByDivRep: UnknownType,

	  AboveSubMixedFrac = MixedFracRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::MixedFracRep| AboveSub : UnknownType,
		SubRangeMixedFrac = MixedFracRepresented<Layout> ? Layout::MixedFracRep: UnknownType,

#pragma endregion Fractional representations

#pragma region Approaching representations

		//(Approaching Towards Zero),((Raw as unsigned) of 0 results in 0.00...1)
    //Not allowed for Pi and E variants(because constant would vanish in infinite approaching slope)
    //RestrictedFloat reinterpretes raw as uint instead of exponental fraction
		ApproachingBottom = ApproachingRepresented<Layout>? Layout::ApproachingBottomRep: UnknownType,

		//(Approaching Away from Zero),((Raw as unsigned) results in 0.99...9)
    //RestrictedFloat reinterpretes raw as uint instead of exponental fraction
		ApproachingTop = ApproachingTopRepresented<Layout>? Layout::ApproachingTopRep: UnknownType,

  #pragma region ApproachingBottom representation divided by ExtraRep

    ApproachingBottomByDiv = ApproachingBottomByDivRepresented<Layout> ?
    Layout::ApproachingBottomByDivRep: UnknownType,
		
  #pragma region ApproachingBottom representation divided by ExtraRep

  #pragma region ApproachingTop representation divided by ExtraRep

    ApproachingTopByDiv = ApproachingTopByDivRepresented<Layout> ?
    Layout::ApproachingTopByDivRep: UnknownType,
		
  #pragma endregion ApproachingTop representation divided by ExtraRep

  // Approaching midpoint from below (fractional part = 0.999… / ExtraRep)
  // Requires EnableApproachingDivided and ExtraRep divisor.
  // When ExtraRep == 1, numerically equals ApproachingTop.
  // Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)-0.0...01)
  // For variant representations : equals (Normal formula)*constant
  #pragma region ApproachingMidLeft

		ApproachingMidLeft = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> ?
    Layout::ApproachingMidLeftRep: UnknownType,

  #pragma endregion ApproachingMidLeft

  // Formula Representation: SignMultiplier * ((Raw as unsigned)+(DecimalOverflow/ExtraRep)+0.0...01)
  #pragma region ApproachingMidRight

		ApproachingMidRight = ApproachingMidRepresented<Layout> && Has_ExtraRepBits<Policy> ?
    Layout::ApproachingMidRightRep: UnknownType,

  #pragma endregion ApproachingMidRight

  // PlusOffset = +ε (approach from below), MinusOffset = −ε (approach from above).
  #pragma region ApproachingOffsets
    
		//SignMultiplier*(NormalNum magnitude) + ApproachingBottom)*(constant multiplier)
		ApproachingBottomPlusOffset = ApproachingOffsetsRepresented<Layout>?
    Layout::ApproachingBottomRep|Layout::PlusOffsetRep:UnknownType,
	
    //SignMultiplier*(NormalNum magnitude + ApproachingTop)*(constant multiplier)
		ApproachingTopPlusOffset = ApproachingOffsetsRepresented<Layout>?
    Layout::ApproachingTopRep|Layout::PlusOffsetRep:UnknownType,
	
    //SignMultiplier*(NormalNum magnitude - ApproachingBottom)*(constant multiplier)
    ApproachingBottomMinusOffset = ApproachingOffsetsRepresented<Layout>?
    Layout::ApproachingBottomRep|Layout::MinusOffsetRep:UnknownType,

		
    //SignMultiplier*(NormalNum magnitude - ApproachingBottom)*(constant multiplier)
    ApproachingTopMinusOffset = ApproachingOffsetsRepresented<Layout>?
    Layout::ApproachingTopRep|Layout::MinusOffsetRep:UnknownType,

  #pragma endregion ApproachingOffsets

#pragma endregion Approaching representations

#pragma region Infinite representations

		Infinity = InfinityRepresented<Layout> ? Layout::InfinityRep : UnknownType,
    ImaginaryInfinity = InfinityRepresented<Layout> ? Layout::InfinityRep| Layout::INumRep : UnknownType,

#pragma endregion Infinite representations

#pragma endregion UniverseUnits representations

    //Bounded to maximum of either 1 magnitude(either wrap around behavior or exception if exceed)
		//Similar to infinity in that doesn't have measureable in real units length
    //but still acts as maximum theorical distance in a universe etc
    UniverseUnits = UniverseUnitsRepresented<Layout> ? Layout::UniverseUnitsRep : UnknownType,
    ImaginaryUniverseUnits = UniverseUnitsRepresented<Layout> ? Layout::UniverseUnitsRep| Layout::INumRep : UnknownType,

#pragma endregion UniverseUnits representations

#pragma region PowerOf representations

		AboveSubPowerOf = PowerOfRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep| AboveSub : UnknownType,
		SubRangePowerOf = PowerOfRepresented<Layout>?
    Layout::ToPowerOfRep: UnknownType,

		AboveSubPowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout::PiNumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfPi = PowerOfRepresented<Layout>&&ExtraPiRepresented<Layout>?
    Layout::ToPowerOfRep|Layout::PiNumExtraRep: UnknownType,
		
		AboveSubPowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfE = PowerOfRepresented<Layout>&&ExtraERepresented<Layout>?
    Layout::ToPowerOfRep|Layout:ENumExtraRep: UnknownType,
		
		AboveSubPowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:INumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout> ? Layout::ToPowerOfRep|Layout:INumExtraRep: UnknownType,
		NegativeSubRangePowerOfI = PowerOfRepresented<Layout>&&ExtraIRepresented<Layout>&&SignEnabled<Layout> ? Layout::ToPowerOfRep|Layout:INumExtraRep|NegativeSign : UnknownType,
		
		AboveSubPowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiENumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfPiE = PowerOfRepresented<Layout>&&ExtraPiERepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiENumExtraRep: UnknownType,
		
		AboveSubPowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfPiI = PowerOfRepresented<Layout>&&ExtraPiIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiINumExtraRep: UnknownType,
		
		AboveSubPowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfEI = PowerOfRepresented<Layout>&&ExtraEIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:EINumExtraRep: UnknownType,
		
		AboveSubPowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep| AboveSub : UnknownType,
		SubRangePowerOfPiEI = PowerOfRepresented<Layout>&&ExtraPiEIRepresented<Layout>?
    Layout::ToPowerOfRep|Layout:PiEINumExtraRep: UnknownType,

#pragma endregion PowerOf representations

#pragma region RootOf representations
    //Does not require ExtraRep since fixed root of
		AboveSubSqrtOf = SqrtOfRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::SqrtOfRep| AboveSub  : UnknownType,
		SubRangeSqrtOf = SqrtOfRepresented<Layout>&&ReciprocalRepresented<Layout>?
    Layout::SqrtOfRep : UnknownType,

    //Requires ExtraRep (same as with PowerOf)
		AboveSubNthRootOf = PowerOfRepresented<Layout>&&SignedExpEnabled<Layout>&&ReciprocalRepresented<Layout>?
    Layout::ToPowerOfRep| AboveSub | Layout::ReciprocalOfRep : UnknownType,
		SubRangeNthRootOf = PowerOfRepresented<Layout>&&ReciprocalRepresented<Layout>?
    Layout::ToPowerOfRep|Layout::ReciprocalOfRep: UnknownType,

#pragma endregion RootOf representations

#pragma region WithinMinMaxRange representations
/*
		//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		WithinMinMaxRange = Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange ? Layout::WithinMinMaxRangeRep : UnknownType,
		//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		NotWithinMinMaxRange = Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange ? Layout::NotWithinMinMaxRangeRep : UnknownType,
*/
#pragma endregion WithinMinMaxRange representations

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

#pragma region Experimental symbolic trig expression flags
		// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
		#pragma region Circular symbolic trig expression flags

		AboveSubSin = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::SinRep| AboveSub : UnknownType,
		SubRangeSin = TrigRepresented<Layout>?
    Layout::SinRep: UnknownType,

		AboveSubCos = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::CosRep| AboveSub : UnknownType,
		SubRangeCos = TrigRepresented<Layout>?
    Layout::CosRep: UnknownType,

		AboveSubTan = TrigRepresented<Layout>&&SignedExpEnabled<Layout>?
    Layout::TanRep| AboveSub : UnknownType,
		SubRangeTan = TrigRepresented<Layout>? Layout::TanRep: UnknownType,

		#pragma endregion Circular symbolic trig expression flags

		#pragma region Hyperbolic symbolic expression flags

		AboveSubSinH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::SinHRep| AboveSub : UnknownType,
		SubRangeSinH = HyperbolicRepresented<Layout> ? Layout::SinHRep: UnknownType,

		AboveSubCosH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::CosHRep| AboveSub : UnknownType,
		SubRangeCosH = HyperbolicRepresented<Layout> ? Layout::CosHRep: UnknownType,

		AboveSubTanH = HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::TanHRep| AboveSub : UnknownType,
		SubRangeTanH = HyperbolicRepresented<Layout> ? Layout::TanHRep: UnknownType,

		#pragma endregion Hyperbolic symbolic expression flags

		#pragma region Inversed expressions

    //Inversed Circular symbolic expression flag
		AboveSubInversedOf = InversionRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::InversedOfRep| AboveSub : UnknownType,
		SubRangeInversedOf = InversionRepresented<Layout>? Layout::InversedOfRep: UnknownType,

    #pragma region ArcSin

		AboveSubCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep| AboveSub | Layout::SinRep : UnknownType,
		SubRangeCsc = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep | Layout::SinRep : UnknownType,

    #pragma endregion ArcSin

    #pragma region ArcCos

		AboveSubArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep| AboveSub | Layout::CosRep : UnknownType,
		SubRangeArcCos = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep | Layout::CosRep: UnknownType,

    #pragma endregion ArcCos

    #pragma region ArcTan

		AboveSubArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep| AboveSub | Layout::TanRep: UnknownType,
		SubRangeArcTan = InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::InversedOfRep | Layout::TanRep: UnknownType,

    #pragma region ArcTan

    #pragma region Hyperbolic ArcSin

		AboveSubCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::InversedOfRep| AboveSub | Layout::SinHRep : UnknownType,
		SubRangeCscH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep | Layout::SinHRep : UnknownType,

    #pragma endregion Hyperbolic ArcSin

    #pragma region Hyperbolic ArcCos
		AboveSubArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep| AboveSub | Layout::CosHRep : UnknownType,
		SubRangeArcCosH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep | Layout::CosHRep: UnknownType,
    #pragma endregion Hyperbolic ArcCos

    #pragma region Hyperbolic ArcTan
		AboveSubArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::InversedOfRep| AboveSub | Layout::TanHRep: UnknownType,
		SubRangeArcTanH = InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep | Layout::TanHRep: UnknownType,
    #pragma region Hyperbolic ArcTan

		#pragma endregion Inversed expressions

		#pragma region Reciprocal expressions

		//Reciprocal expression flag 1/OtherExpression/Value
		AboveSubReciprocalOf = ReciprocalRepresented<Layout>&&SignedExpEnabled<Layout> ? Layout::ReciprocalOfRep| AboveSub : UnknownType,
		SubRangeReciprocalOf = ReciprocalRepresented<Layout> ? Layout::ReciprocalOfRep: UnknownType,

    //cosecant(x) == 1/sin(x)
    #pragma region cosecant

		AboveSubCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep| AboveSub | Layout::SinRep : UnknownType,
		SubRangeCsc = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::SinRep : UnknownType,

    #pragma endregion cosecant

    //secant(x) = 1/cos(x)
    #pragma region secant

		AboveSubSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep| AboveSub | Layout::CosRep : UnknownType,
		SubRangeSec = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::CosRep: UnknownType,

    #pragma endregion secant

    //Cotangent = 1/tan(x)
    #pragma region Cotangent

		AboveSubCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep| AboveSub | Layout::TanRep: UnknownType,
		SubRangeCot = ReciprocalRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::TanRep: UnknownType,

    #pragma region Cotangent

    #pragma region Hyperbolic cosecant

		AboveSubCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::ReciprocalOfRep| AboveSub | Layout::SinHRep : UnknownType,
		SubRangeCscH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::SinHRep : UnknownType,

    #pragma endregion Hyperbolic cosecant

    #pragma region Hyperbolic secant

		AboveSubSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep| AboveSub | Layout::CosHRep : UnknownType,
		SubRangeSecH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::CosHRep: UnknownType,

    #pragma endregion Hyperbolic secant

    #pragma region Hyperbolic Cotangent

		AboveSubCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep| AboveSub | Layout::TanHRep: UnknownType,
		SubRangeCotH = ReciprocalRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep | Layout::TanHRep: UnknownType,

    #pragma region Hyperbolic Cotangent

		#pragma endregion Reciprocal expressions

    #pragma region Arc cosecant

		AboveSubArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep| AboveSub | Layout::SinRep : UnknownType,
		SubRangeArcCsc = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep | Layout::SinRep : UnknownType,

    #pragma endregion Arc cosecant

    #pragma region Arc secant

		AboveSubArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep| AboveSub | Layout::CosRep : UnknownType,
		SubRangeArcCos = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep | Layout::CosRep: UnknownType,

    #pragma endregion ArcCos

    #pragma region Arc cotangent

		AboveSubArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep| AboveSub | Layout::TanRep: UnknownType,
		SubRangeArcTan = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&TrigRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep | Layout::TanRep: UnknownType,

    #pragma region Arc cotangent

    #pragma region Hyperbolic Arc cosecant

		AboveSubCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout> &&
    ? Layout::InversedOfRep| AboveSub | Layout::SinHRep : UnknownType,
		SubRangeCscH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::InversedOfRep | Layout::SinHRep : UnknownType,

    #pragma endregion Hyperbolic ArcSin

    #pragma region Hyperbolic Arc secant

		AboveSubArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep| AboveSub | Layout::CosHRep : UnknownType,
		SubRangeArcCosH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep | Layout::CosHRep: UnknownType,

    #pragma endregion Hyperbolic Arc secant

    #pragma region Hyperbolic Arc cotangent

		AboveSubArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>&&SignedExpEnabled<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep| AboveSub | Layout::TanHRep: UnknownType,
		SubRangeArcTanH = ReciprocalRepresented<Layout>&&InversionRepresented<Layout>&&HyperbolicRepresented<Layout>
    ? Layout::ReciprocalOfRep|Layout::InversedOfRep | Layout::TanHRep: UnknownType,

    #pragma region Hyperbolic Arc cotangent

#pragma endregion Experimental symbolic trig expression flags

#pragma region Indeterminate forms
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
		InfinityByInfinity = Layout::IndeterminateFormRep!=0&&(Layout::InfinityByInfinityRep > IndeterminateFormRep) ? Layout::InfinityByInfinityRep: UnknownType,
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
#pragma endregion Indeterminate forms
  },
	
}