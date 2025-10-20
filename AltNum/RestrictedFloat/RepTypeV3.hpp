// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//#include "..\DLLAPI.h"
#include <cstdint>
#include <concepts>

namespace RepTypeV3Detection {
  //UnknownTypeOverride is required to be greater than all other values
	template<typename Policy>
	concept Has_UnknownType = requires { { Policy::UnknownTypeOverride }   -> std::convertible_to<unsigned>; };

	template<typename Policy>
	concept Has_EnablePi   = requires { { Policy::EnablePi }   -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableE    = requires { { Policy::EnableE }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableI    = requires { { Policy::EnableI }    -> std::convertible_to<bool>; };
  template<typename Policy>
  concept Has_EnablePiE = requires {
    { Policy::EnablePi } -> std::convertible_to<bool>;
    { Policy::EnableE }  -> std::convertible_to<bool>;
  };
  template<typename Policy>
  concept Has_EnableEI = requires {
    { Policy::EnableE }  -> std::convertible_to<bool>;
    { Policy::EnableI }    -> std::convertible_to<bool>;
  };
  template<typename Policy>
  concept Has_EnablePiEI = requires {
    { Policy::EnablePi } -> std::convertible_to<bool>;
    { Policy::EnableE }  -> std::convertible_to<bool>;
    { Policy::EnableI }    -> std::convertible_to<bool>;
  };

	template<typename Policy>
	concept Has_EnablePhi    = requires { { Policy::EnablePhi }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableLn2Num    = requires { { Policy::EnableLn2Num }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnablePlasticNum    = requires { { Policy::EnablePlasticNum }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableIndeterminateForms    = requires { { Policy::EnableIndeterminateForms }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableFractionals    = requires { { Policy::EnableFractionals }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableApproaching    = requires { { Policy::EnableApproaching }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableApproachingTop    = requires { { Policy::EnableApproachingTop }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableApproachingDivided    = requires {
    { Policy::EnableFractionals }    -> std::convertible_to<bool>;
    { Policy::EnableApproaching }    -> std::convertible_to<bool>;
    { Policy::EnableApproachingDivided }    -> std::convertible_to<bool>;
  };
	template<typename Policy>
	concept Has_EnableApproachingOffsets    = requires {
    { Policy::EnableApproaching }    -> std::convertible_to<bool>;
    { Policy::EnableApproachingOffsets }    -> std::convertible_to<bool>;
  };

	template<typename Policy>
	concept Has_EnableTrigExpressions    = requires { { Policy::EnableTrigExpressions }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableInfinity    = requires { { Policy::EnableInfinity }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnablePowerOf    = requires { { Policy::EnablePowerOf }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnablePowerOfV2    = requires { { Policy::EnablePowerOfV2 }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableMixedFrac    = requires { { Policy::EnableMixedFrac }    -> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_EnableWithinMinMaxRange    = requires { { Policy::EnableWithinMinMaxRange }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableNotWithinMinMaxRange    = requires { { Policy::EnableNotWithinMinMaxRange }    -> std::convertible_to<bool>; };

}

namespace BlazesRusCode
{
	/*
    // RepType: Enumerates the *current* representation type of a value.
    // Describes its canonical stored form after normalisation — not the
    // sequence of operations that produced it.
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
	       ahead of time; representation reflects current stored form.
	
	     PlusOffset  (Fixed + ApproachingX(...))
	     MinusOffset (Fixed − ApproachingX(...))

		IndeterminateForm:
				→ Given priority over other flags; subtype bits may also be set.

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
		//real root of (x^3 = x+1x^3 = x + 1)
		// PlasticSquareNum * PlasticNum is  reduced to (ρ + 1).
		//ρ is tied to Padovan and Perrin sequences
		
		//For imaginary variants of any representation represents: (NormalFormula)*imaginary number
	*/
  template<class Policy, class Layout>
  enum class RepTypeV3 : uint32_t {
		NormalType = 0,
		// top bit sentinel flag
    // Reserved: highest bit in uint32_t, used only as 'unknown' sentinel.
    // No real category will be assigned to this slot.
		UnknownType = RepTypeV3Detection::Has_UnknownType<Layout>?Layout::UnknownTypeOverride:(1u << 31);
		
		//Core multipliers
    PiNum    = RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ?
    Layout::PiNumRep : UnknownType,//(1u << 1)
    ENum    = RepTypeV3Detection::Has_EnableE<Policy>&&Policy::EnableE ?
    Layout::ENumRep : UnknownType,//(1u << 2)
    // Magnitude * imaginary number representation
    INum    = RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ?
    Layout::INumRep : UnknownType,//(1u << 3)
		
		//Core composite representations
    PiENum    = RepTypeV3Detection::Has_EnablePiE<Policy>&&Policy::EnablePi &&
    Policy::EnableE ? PiNum|ENum : UnknownType,
    PiINum    = RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi &&
    Policy::EnableI ? PiNum|INum : UnknownType,
    EINum    = RepTypeV3Detection::Has_EnableEI<Policy>&&Policy::EnableE &&
    Policy::EnableI ? ENum|INum : UnknownType,
    PiEINum    = RepTypeV3Detection::Has_EnablePiEI<Policy>&&Policy::EnablePi &&
    Policy::EnableE &&Policy::EnableI ? PiNum|PiENum|INum : UnknownType,

		NumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals ?
    Layout::NumByDivRep : UnknownType,
		//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
    //Not allowed for Pi and E variants(because constant would vanish in infinite approaching slope)
		ApproachingBottom = RepTypeV3Detection::Has_EnableApproaching<Policy>&&Policy::EnableApproaching ?
    Layout::ApproachingBottomRep : UnknownType,
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		ApproachingTop = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop ?
    Layout::ApproachingTopRep : UnknownType,

		//ApproachingBottom representation/ExtraRep
		ApproachingBottomByDiv = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals ? ApproachingBottom+NumByDiv : UnknownType,
		//ApproachingTop representation/ExtraRep
		ApproachingTopByDiv = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals ? ApproachingTop+NumByDiv : UnknownType,
    // Approaching midpoint from below (fractional part = 0.999… / ExtraRep)
    // Requires EnableApproachingDivided and ExtraRep divisor.
    // When ExtraRep == 1, numerically equals ApproachingTop.
		// Formula Representation: SignMultiplier * ((Magnitude of IntHalf)+(DecimalOverflow/ExtraRep)-0.0...01)
		// For variant representations : equals (Normal formula)*constant
		ApproachingMidLeft = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided ?
    Layout::ApproachingMidLeftRep : UnknownType,
		// Formula Representation: SignMultiplier * ((Magnitude of IntHalf)+(DecimalOverflow/ExtraRep)+0.0...01)
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided ?
    Layout::ApproachingMidRightRep : UnknownType,
    //Offset flags for Approaching offset(these aren't used as actual representation usually)
    PlusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching ? Layout::PlusOffsetRep : UnknownType,
    MinusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching ? Layout::MinusOffsetRep : UnknownType,
		//For non-I based variants of Approaching offset represents (SignMultiplier*magnitude) +- Approaching*
    // Approaching offset flags:
    // These indicate an infinitesimal +ε or −ε adjustment to the base magnitude.
    // Used for special cases where the base IntHalf/DecimalHalf is fixed,
    // but approach semantics still apply.
    // PlusOffset = +ε (approach from below), MinusOffset = −ε (approach from above).
		//SignMultiplier*(NormalNum magnitude + ApproachingBottom)
		ApproachingBottomPlusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    RepTypeV3Detection::EnableApproachingTop<Policy>&&Policy::EnableApproaching ? ApproachingBottom|PlusOffset : UnknownType,
		//SignMultiplier*(NormalNum magnitude + ApproachingTop)
		ApproachingTopPlusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    RepTypeV3Detection::EnableApproachingTop<Policy>&&Policy::EnableApproachingTop ? ApproachingTop|PlusOffset : UnknownType,
		//SignMultiplier*(NormalNum magnitude - ApproachingBottom)
		ApproachingBottomMinusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching ? ApproachingBottom|MinusOffset : UnknownType,
		//SignMultiplier*(NormalNum magnitude - ApproachingTop)
		ApproachingTopMinusOffset = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    RepTypeV3Detection::EnableApproachingTop<Policy>&&Policy::EnableApproachingTop ? ApproachingTop|MinusOffset : UnknownType,

		//Sign determines if either positive or negative infinity
		Infinity = RepTypeV3Detection::Has_EnableInfinity<Policy>&&Policy::EnableInfinity ? Layout::InfinityRep : UnknownType,
    ImaginaryInfinity = RepTypeV3Detection::Has_EnableInfinity<Policy>&&Policy::EnableInfinity &&
    RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ? Infinity|INum  : UnknownType,

		//Current format of AltDec only allows either ToPowerOf, MixedFrac, or (WithinMinMaxRange/NotWithinMinMaxRange) feature enabled
		//Value to power of ExtraRep
		ToPowerOf = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf ? Layout::ToPowerOfRep : UnknownType,
		//IntValue to power of DecimalHalf/ExtraRep(Experimental Representation--Only for NormalNum variants)
		ToPowerOfV2 = RepTypeV3Detection::Has_EnablePowerOfV2<Policy>&&Policy::EnablePowerOfV2 ? Layout::ToPowerOfV2Rep : UnknownType,
		//Represents magnitude of IntHalf_DecimalHalf/ExtraRep
		MixedFrac = RepTypeV3Detection::Has_EnableMixedFrac && Policy::EnableMixedFrac ? Layout::MixedFracRep : UnknownType,

		//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		WithinMinMaxRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange ? Layout::WithinMinMaxRangeRep : UnknownType,
		//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		NotWithinMinMaxRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange ? Layout::NotWithinMinMaxRangeRep : UnknownType,

    //Other math constants
	  PhiNum    = RepTypeV3Detection::Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::PhiNumRep : UnknownType,
		//√5
	  SqrtFiveNum    = RepTypeV3Detection::Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::SqrtFiveNumRep : UnknownType,
		PhiConjugateNum  = RepTypeV3Detection::Has_EnablePhi<Policy>&&Policy::EnablePhi ? Layout::DividedByPhiNumRep : UnknownType,
    //ln(2)
	  Ln2Num    = RepTypeV3Detection::Has_EnableLn2Num<Policy>&&Policy::EnableLn2 ? Layout::Ln2Rep : UnknownType
	  PlasticNum    = RepTypeV3Detection::Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum ? Layout::PlasticNumRep : UnknownType
	  PlasticSquareNum    = RepTypeV3Detection::Has_EnablePlasticSquareNum<Policy>&&Policy::EnablePlasticSquareNum ? Layout::PlasticSquareNumRep : UnknownType

		// Experimental symbolic trig expression flags.
		// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
		#pragma region Circular symbolic trig expression flags
		Sin = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::SinRep : UnknownType,
		Cos = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::CosRep : UnknownType,
		Tan = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::TanRep : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		//Hyperbolic equivant flag
		HyperbolicFunction = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::HyperbolicFlag : UnknownType,
		#pragma region Hyperbolic symbolic expression flags
		SinH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Sin|HyperbolicFunction : UnknownType,
		CosH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Cos|HyperbolicFunction : UnknownType,
		TanH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Tan|HyperbolicFunction : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		//Inversed Circular symbolic expression flag
		InversedFunction = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::InverseFlag : UnknownType,
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSin = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Sin|InversedFlag : UnknownType,
		ArcCos = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Cos|InversedFlag : UnknownType,
		ArcTan = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Tan|InversedFlag : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? SinH|InversedFlag : UnknownType,
		ArcCosH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? CosH|InversedFlag : UnknownType,
		ArcTanH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? TanH|InversedFlag : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		//Reciprocal expression flag 1/OtherExpression
		ReciprocalFunction = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Layout::ReciprocalFunctionRep : UnknownType,
		#pragma region Circular reciprocal trig expression flags 
		//cosecant(x) == 1/sin(x)
		Csc = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Sin|ReciprocalFunction : UnknownType,
		//secant(x) = 1/cos(x)
		Sec = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Cos|ReciprocalFunction : UnknownType,
		//Cotangent = 1/tan(x)
		Cot = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? Tan|ReciprocalFunction : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? SinH|ReciprocalFunction : UnknownType,
		SecH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? CosH|ReciprocalFunction : UnknownType,
		CotH = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions ? TanH|ReciprocalFunction : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags
	  // --------------------
    // Indeterminate forms(Required to be highest bit value from Layout)
    // Requires enough bits set in layout in order for all Indeterminate forms to be enabled
    // --------------------
    // All share the IndeterminateForm bit(These act as sentinal not just as flags)
    // Additional bits indicate the specific undefined/indeterminate case.
    // Checked first when EnableIndeterminateForms is enabled.
		IndeterminateForm = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms ? 
    IndeterminateFormRep: UnknownType,
		NaN = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::NaNRep > IndeterminateFormRep) ? 
    Layout::NaNRep : UnknownType,//IndeterminateForm | (1u << 1)
		Nil = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::NilRep > IndeterminateFormRep) ?
    Layout::NilRep : UnknownType,//IndeterminateForm | (1u << 2)
		Undefined = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::UndefinedRep > IndeterminateFormRep) ? 
    Layout::UndefinedRep : UnknownType,//IndeterminateForm | (1u << 3)
		DividedByZero = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::DividedByZeroRep > IndeterminateFormRep) ?
    Layout::DividedByZeroRep : UnknownType,//IndeterminateForm | (1u << 4)
		//0/0
		ZeroByZero = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::ZeroByZeroRep > IndeterminateFormRep) ?
    Layout::ZeroByZeroRep : UnknownType,
		//0 to power of 0
		ZeroToZeroPower = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::ZeroToZeroPowerRep > IndeterminateFormRep) ?
    Layout::ZeroToZeroPowerRep : UnknownType,
		//∞/∞ Representation
		//NegativeInfinityByInfinity represented if Sign bit is negative
		InfinityByInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InfinityByInfinityRep > IndeterminateFormRep) ?
    Layout::InfinityByInfinityRep : UnknownType,
		InfinityMinusInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InfinityMinusInfinityRep > IndeterminateFormRep) ?
    Layout::InfinityMinusInfinityRep : UnknownType,
		//∞^∞ Representation
		//InfinityPowerOfInfinity ∞^∞ — extreme divergence; useful to trap before overflow.
		InfinityPowerOfInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InfinityPowerOfInfinityRep > IndeterminateFormRep) ?
    Layout::InfinityPowerOfInfinityRep : UnknownType,
		InfinityPowerOfNegativeInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InfinityPowerOfNegativeInfinityRep > IndeterminateFormRep) ?
    Layout::InfinityPowerOfNegativeInfinity : UnknownType,
		//NegativeOnePowerOfInfinity represented if Sign bit is negative
		//NegativeOnePowerOfInfinity (-1)^∞ — oscillates between ±1;
		OnePowerOfInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OnePowerOfInfinityRep > IndeterminateFormRep) ?
    Layout::OnePowerOfInfinityRep : UnknownType,
		InfinityToZeroPower = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InfinityToZeroPowerRep > IndeterminateFormRep) ?
    Layout::InfinityToZeroPowerRep : UnknownType,
		//ZeroPowerOfNegativeInfinity 0^{−∞} — diverges to ∞; distinct from ∞^∞.
		ZeroPowerOfNegativeInfinity = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::DividedByZeroRep > IndeterminateFormRep) ?
    Layout::ZeroPowerOfNegativeInfinityRep : UnknownType,//IndeterminateForm | (1u << 4)
		UndefinedButWithinZeroInfinityRange = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::UndefinedButWithinZeroInfinityRangeRep > IndeterminateFormRep) ?
    Layout::UndefinedButWithinZeroInfinityRangeRep : UnknownType,//IndeterminateForm | (1u << 4)
    //oscillatory, no limit(Given priority over later flags same as IndeterminateForm)
		//Sin(1/x) as x→0 — oscillatory, no limit if FlagState == 1(Pi)
		//Cos(1/x) as x→0 — oscillatory, no limit if FlagState == 2(E)
		//Tan(1/x) as x→0 — oscillatory, no limit if FlagState == 3(Tan)
		OscillatoryNoLimit = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) ?
    Layout::OscillatoryNoLimitRep : UnknownType,
		//OscillatoryNonTrig — non-periodic oscillations not tied to sin/cos/tan, e.g. sgn(sin(1/x)).
		OscillatoryNonTrig = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OscillatoryNonTrigRep > IndeterminateFormRep) ?
    Layout::OscillatoryNonTrigRep : UnknownType,
		//PathDependentLimit — result depends on complex-plane approach path.
		PathDependentLimit = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::PathDependentLimitRep > IndeterminateFormRep) ?
    Layout::PathDependentLimitRep : UnknownType,
		//ChaoticNoLimit — bounded but aperiodic behaviour (rare, but trap‑worthy).
		ChaoticNoLimit = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::ChaoticNoLimitRep > IndeterminateFormRep) ?
    Layout::ChaoticNoLimitRep : UnknownType,
		//PrecisionLossUndefined — internal catastrophic cancellation or overflow kills validity.
		PrecisionLossUndefined = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::PrecisionLossUndefinedRep > IndeterminateFormRep) ?
    Layout::PrecisionLossUndefinedRep : UnknownType,
		//EvenRootOfNegative — e.g., √[even]{−5} over ℝ.
		EvenRootOfNegative = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::EvenRootOfNegativeRep > IndeterminateFormRep) ?
    Layout::EvenRootOfNegativeRep : UnknownType,
		//Result is a potential complex number which is not currently supported by current class
    //log(−x) — undefined in without complex numbers; valid only with complex number support.
		UnsupportedComplexNumberResult = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::UnsupportedComplexNumberResultRep > IndeterminateFormRep) ?
    Layout::UnsupportedComplexNumberResultRep : UnknownType,
		//poles of Γ(z).
		FactorialOfNegativeNonInteger  = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::FactorialOfNegativeNonIntegerRep > IndeterminateFormRep) ?
    Layout::FactorialOfNegativeNonIntegerRep : UnknownType,
		//e.g., arcsin(2) in R.
		InverseTrigOutOfRange = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::InverseTrigOutOfRangeRep > IndeterminateFormRep) ?
    Layout::InverseTrigOutOfRangeRep : UnknownType,
    //UnimplementedOperation — not mathematically undefined, just not yet coded.
		UnimplementedOperation = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::UnimplementedOperationRep > IndeterminateFormRep) ?
    Layout::UnimplementedOperationRep : UnknownType,
		//DomainError — generic “arg outside allowed domain” catch-all.
		DomainError = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::DomainErrorRep > IndeterminateFormRep) ?
    Layout::DomainErrorRep : UnknownType,
		//--PiNum Variants--
		PiNumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ? Layout::PiNumByDivRep : UnknownType,

		ApproachingTopPi = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ? ApproachingTopPi : UnknownType,

		ApproachingBottomByDivPi = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePi<Policy> &&
		Policy::EnablePi ? ApproachingBottomByDiv|PiNum : UnknownType,
		ApproachingTopByDivPi = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePi<Policy> &&
    Policy::EnablePi ? ApproachingTopByDiv|PiNum : UnknownType,

		ApproachingMidLeftPi = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ? ApproachingMidLeft|PiNum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ? ApproachingMidRight|PiNum : UnknownType,

		ApproachingBottomPlusOffsetPi = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ?
    ApproachingBottomPlusOffset|PiNum : UnknownType,
		
		ApproachingTopPlusOffsetPi = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ?
    ApproachingTopPlusOffset|PiNum : UnknownType,
		
		ApproachingBottomMinusOffsetPi = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ?
    ApproachingBottomMinusOffset|PiNum : UnknownType,
		
		ApproachingTopMinusOffsetPi = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi ?
    ApproachingTopMinusOffset|PiNum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Sin|PiNum : UnknownType,
		CosPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Cos|PiNum : UnknownType,
		TanPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Tan|PiNum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? SinH|PiNum : UnknownType,
		CosHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? CosH|PiNum : UnknownType,
		TanHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? TanH|PiNum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcSin|PiNum : UnknownType,
		ArcCosPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcCos|PiNum : UnknownType,
		ArcTanPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcTan|PiNum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcSinH|PiNum : UnknownType,
		ArcCosHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcCosH|PiNum : UnknownType,
		ArcTanHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? ArcTanH|PiNum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Csc|PiNum : UnknownType,
		SecPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Sec|PiNum : UnknownType,
		CotPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? Cot|PiNum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? CscH|PiNum : UnknownType,
		SecHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? SecH|PiNum : UnknownType,
		CotHPi = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePi && Policy::EnablePi ? CotH|PiNum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

		//(Value*Pi)^ExtraRep
    PiPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi? ToPowerOf|PiNum  : UnknownType,

    WithinMinMaxPiRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ? WithinMinMaxRange|PiNum : UnknownType,
		NotWithinMinMaxPiRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ? NotWithinMinMaxRange|PiNum : UnknownType,

		MixedPi = RepTypeV3Detection::Has_EnableMixedFrac && Policy::EnableMixedFrac &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ? MixedFrac|PiNum : UnknownType,

		OscillatoryNoLimitPi = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi ? Layout::OscillatoryNoLimitRep|PiNum : UnknownType,
		//------------------
		//--ENum Variants--
		ENumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnableE<Policy>&&Policy::EnableE ? Layout::ENumByDivRep : UnknownType,

		ApproachingTopE = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? ApproachingTopE|ENum : UnknownType,

		ApproachingBottomByDivE = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableE<Policy> &&
		Policy::EnableE ? ApproachingBottomByDiv|ENum : UnknownType,
		ApproachingTopByDivE = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableE<Policy> &&
    Policy::EnableE ? ApproachingTopByDiv|ENum : UnknownType,

		ApproachingMidLeftE = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? ApproachingMidLeft|ENum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? ApproachingMidRight|ENum : UnknownType,

		ApproachingBottomPlusOffsetE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ?
    ApproachingBottomPlusOffset|ENum : UnknownType,
		
		ApproachingTopPlusOffsetE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ?
    ApproachingTopPlusOffset|ENum : UnknownType,
		
		ApproachingBottomMinusOffsetE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ?
    ApproachingBottomMinusOffset|ENum : UnknownType,
		
		ApproachingTopMinusOffsetE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ?
    ApproachingTopMinusOffset|ENum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Sin|ENum : UnknownType,
		CosE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Cos|ENum : UnknownType,
		TanE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Tan|ENum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? SinH|ENum : UnknownType,
		CosHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? CosH|ENum : UnknownType,
		TanHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? TanH|ENum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcSin|ENum : UnknownType,
		ArcCosE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcCos|ENum : UnknownType,
		ArcTanE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcTan|ENum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcSinH|ENum : UnknownType,
		ArcCosHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcCosH|ENum : UnknownType,
		ArcTanHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? ArcTanH|ENum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Csc|ENum : UnknownType,
		SecE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Sec|ENum : UnknownType,
		CotE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? Cot|ENum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? CscH|ENum : UnknownType,
		SecHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? SecH|ENum : UnknownType,
		CotHE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableE && Policy::EnableE ? CotH|ENum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    EPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? ToPowerOf|ENum  : UnknownType,

    WithinMinMaxERange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? WithinMinMaxRange|ENum : UnknownType,
		NotWithinMinMaxERange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? NotWithinMinMaxRange|ENum : UnknownType,

		MixedE = RepTypeV3Detection::Has_EnableMixedFrac && Policy::EnableMixedFrac &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? MixedFrac|ENum : UnknownType,

		OscillatoryNoLimitE = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) &&
    RepTypeV3Detection::Has_EnableE<Policy> && Policy::EnableE ? Layout::OscillatoryNoLimitRep|ENum : UnknownType,
		//------------------
		//--INum Variants--
		INumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ? Layout::INumByDivRep : UnknownType,

		ApproachingBottomI = RepTypeV3Detection::Has_EnableApproaching<Policy>&&Policy::EnableApproaching && 
    RepTypeV3Detection::Has_EnableI<Policy> && RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ? ApproachingBottom|INum : UnknownType,
		ApproachingTopI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnableI<Policy> && RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ? ApproachingTopI : UnknownType,

		ApproachingBottomByDivI = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableI<Policy> &&
		RepTypeV3Detection::Has_EnableI<Policy>&&Policy::EnableI ? ApproachingBottomByDiv|INum : UnknownType,
		ApproachingTopByDivI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? ApproachingTopByDiv|INum : UnknownType,

		ApproachingMidLeftI = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? ApproachingMidLeft|INum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? ApproachingMidRight|INum : UnknownType,

		ApproachingBottomPlusOffsetI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ?
    ApproachingBottomPlusOffset|INum : UnknownType,
		
		ApproachingTopPlusOffsetI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ?
    ApproachingTopPlusOffset|INum : UnknownType,
		
		ApproachingBottomMinusOffsetI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ?
    ApproachingBottomMinusOffset|INum : UnknownType,
		
		ApproachingTopMinusOffsetI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ?
    ApproachingTopMinusOffset|INum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Sin|INum : UnknownType,
		CosI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Cos|INum : UnknownType,
		TanI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Tan|INum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? SinH|INum : UnknownType,
		CosHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? CosH|INum : UnknownType,
		TanHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? TanH|INum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcSin|INum : UnknownType,
		ArcCosI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcCos|INum : UnknownType,
		ArcTanI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcTan|INum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcSinH|INum : UnknownType,
		ArcCosHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcCosH|INum : UnknownType,
		ArcTanHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? ArcTanH|INum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Csc|INum : UnknownType,
		SecI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Sec|INum : UnknownType,
		CotI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? Cot|INum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? CscH|INum : UnknownType,
		SecHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? SecH|INum : UnknownType,
		CotHI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableI && Policy::EnableI ? CotH|INum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    IPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? ToPowerOf|INum  : UnknownType,

    WithinMinMaxIRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? WithinMinMaxRange|INum : UnknownType,
		NotWithinMinMaxIRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? NotWithinMinMaxRange|INum : UnknownType,

		OscillatoryNoLimitI = RepTypeV3Detection::Has_EnableIndeterminateForms<Policy>&&Policy::EnableIndeterminateForms&&(Layout::OscillatoryNoLimitRep > IndeterminateFormRep) &&
    RepTypeV3Detection::Has_EnableI<Policy> && Policy::EnableI ? Layout::OscillatoryNoLimitRep|INum : UnknownType,
		//------------------
		//--PiENum Variants--
		PiENumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnablePiE<Policy>&&Policy::EnablePi&&Policy::EnableE ? Layout::PiNumByDivRep : UnknownType,

		ApproachingTopPiE = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ? ApproachingTopPiE : UnknownType,

		ApproachingBottomByDivPiE = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiE<Policy> &&
		Policy::EnablePi && Policy::EnableE ? ApproachingBottomByDiv|PiENum : UnknownType,
		ApproachingTopByDivPiE = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiE<Policy> &&
    Policy::EnablePi && Policy::EnableE ? ApproachingTopByDiv|PiENum : UnknownType,

		ApproachingMidLeftPiE = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ? ApproachingMidLeft|PiENum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ? ApproachingMidRight|PiENum : UnknownType,

		ApproachingBottomPlusOffsetPiE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ?
    ApproachingBottomPlusOffset|PiENum : UnknownType,
		
		ApproachingTopPlusOffsetPiE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ?
    ApproachingTopPlusOffset|PiENum : UnknownType,
		
		ApproachingBottomMinusOffsetPiE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ?
    ApproachingBottomMinusOffset|PiENum : UnknownType,
		
		ApproachingTopMinusOffsetPiE = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ?
    ApproachingTopMinusOffset|PiENum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Sin|PiENum : UnknownType,
		CosPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Cos|PiENum : UnknownType,
		TanPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Tan|PiENum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? SinH|PiENum : UnknownType,
		CosHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? CosH|PiENum : UnknownType,
		TanHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? TanH|PiENum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcSin|PiENum : UnknownType,
		ArcCosPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcCos|PiENum : UnknownType,
		ArcTanPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcTan|PiENum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcSinH|PiENum : UnknownType,
		ArcCosHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcCosH|PiENum : UnknownType,
		ArcTanHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? ArcTanH|PiENum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Csc|PiENum : UnknownType,
		SecPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Sec|PiENum : UnknownType,
		CotPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? Cot|PiENum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? CscH|PiENum : UnknownType,
		SecHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? SecH|PiENum : UnknownType,
		CotHPiE = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiE && Policy::EnablePi && Policy::EnableE ? CotH|PiENum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    PiEPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnablePi<Policy>&&Policy::EnablePi &&
    RepTypeV3Detection::Has_EnableE<Policy>&&Policy::EnableE ? ToPowerOf|PiNum|ENum  : UnknownType,

    WithinMinMaxPiERange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiE<Policy> && Policy::EnablePi && Policy::EnableE ? WithinMinMaxRange|PiENum : UnknownType,
		NotWithinMinMaxPiERange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiE<Policy> &&Policy::EnablePi && Policy::EnableE ? NotWithinMinMaxRange|PiENum : UnknownType,

		MixedPiE = RepTypeV3Detection::Has_EnableMixedFrac && Policy::EnableMixedFrac &&
    RepTypeV3Detection::Has_EnablePi<Policy> && Policy::EnablePi && Policy::EnableE ? MixedFrac|PiENum : UnknownType,
		//------------------
		//--PiINum Variants--
		PiINumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnablePiI<Policy>&&Policy::EnablePi&&Policy::EnableI ? Layout::PiNumByDivRep : UnknownType,

		ApproachingTopPiI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? ApproachingTopPiI : UnknownType,

		ApproachingBottomByDivPiI = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiI<Policy> &&
		Policy::EnablePi && Policy::EnableI ? ApproachingBottomByDiv|PiINum : UnknownType,
		ApproachingTopByDivPiI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiI<Policy> &&
    Policy::EnablePi && Policy::EnableI ? ApproachingTopByDiv|PiINum : UnknownType,

		ApproachingMidLeftPiI = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? ApproachingMidLeft|PiINum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? ApproachingMidRight|PiINum : UnknownType,

		ApproachingBottomPlusOffsetPiI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ?
    ApproachingBottomPlusOffset|PiINum : UnknownType,
		
		ApproachingTopPlusOffsetPiI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ?
    ApproachingTopPlusOffset|PiINum : UnknownType,
		
		ApproachingBottomMinusOffsetPiI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ?
    ApproachingBottomMinusOffset|PiINum : UnknownType,
		
		ApproachingTopMinusOffsetPiI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ?
    ApproachingTopMinusOffset|PiINum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Sin|PiINum : UnknownType,
		CosPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Cos|PiINum : UnknownType,
		TanPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Tan|PiINum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? SinH|PiINum : UnknownType,
		CosHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? CosH|PiINum : UnknownType,
		TanHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? TanH|PiINum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcSin|PiINum : UnknownType,
		ArcCosPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcCos|PiINum : UnknownType,
		ArcTanPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcTan|PiINum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcSinH|PiINum : UnknownType,
		ArcCosHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcCosH|PiINum : UnknownType,
		ArcTanHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? ArcTanH|PiINum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Csc|PiINum : UnknownType,
		SecPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Sec|PiINum : UnknownType,
		CotPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? Cot|PiINum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? CscH|PiINum : UnknownType,
		SecHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? SecH|PiINum : UnknownType,
		CotHPiI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiI && Policy::EnablePi && Policy::EnableI ? CotH|PiINum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    PiIPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? ToPowerOf|PiINum  : UnknownType,

    WithinMinMaxPiIRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? WithinMinMaxRange|PiINum : UnknownType,
		NotWithinMinMaxPiIRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiI<Policy> && Policy::EnablePi && Policy::EnableI ? NotWithinMinMaxRange|PiINum : UnknownType,

		//------------------
		//--EINum Variants--
		EINumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnableEI<Policy>&&Policy::EnableE&&Policy::EnableI ? Layout::ENumByDivRep : UnknownType,

		ApproachingTopEI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ? ApproachingTopEI : UnknownType,

		ApproachingBottomByDivEI = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableEI<Policy> &&
		Policy::EnableE && Policy::EnableI ? ApproachingBottomByDiv|EINum : UnknownType,
		ApproachingTopByDivEI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnableEI<Policy> &&
    Policy::EnableE && Policy::EnableI ? ApproachingTopByDiv|EINum : UnknownType,

		ApproachingMidLeftEI = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ? ApproachingMidLeft|EINum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ? ApproachingMidRight|EINum : UnknownType,

		ApproachingBottomPlusOffsetEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ?
    ApproachingBottomPlusOffset|EINum : UnknownType,
		
		ApproachingTopPlusOffsetEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ?
    ApproachingTopPlusOffset|EINum : UnknownType,
		
		ApproachingBottomMinusOffsetEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ?
    ApproachingBottomMinusOffset|EINum : UnknownType,
		
		ApproachingTopMinusOffsetEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnableEI<Policy> && Policy::EnableE && Policy::EnableI ?
    ApproachingTopMinusOffset|EINum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Sin|EINum : UnknownType,
		CosEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Cos|EINum : UnknownType,
		TanEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Tan|EINum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? SinH|EINum : UnknownType,
		CosHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? CosH|EINum : UnknownType,
		TanHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? TanH|EINum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcSin|EINum : UnknownType,
		ArcCosEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcCos|EINum : UnknownType,
		ArcTanEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcTan|EINum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcSinH|EINum : UnknownType,
		ArcCosHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcCosH|EINum : UnknownType,
		ArcTanHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? ArcTanH|EINum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Csc|EINum : UnknownType,
		SecEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Sec|EINum : UnknownType,
		CotEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? Cot|EINum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? CscH|EINum : UnknownType,
		SecHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? SecH|EINum : UnknownType,
		CotHEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnableEI && Policy::EnableE && Policy::EnableI ? CotH|EINum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    EIPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnableEI<Policy>&& Policy::EnableE && Policy::EnableI ? ToPowerOf|ENum|INum  : UnknownType,

    WithinMinMaxEIRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableEI<Policy>&& Policy::EnableE && Policy::EnableI ? WithinMinMaxRange|EINum : UnknownType,
		NotWithinMinMaxEIRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnableEI<Policy>&& Policy::EnableE && Policy::EnableI ? NotWithinMinMaxRange|EINum : UnknownType,

		//------------------
		//--PiEINum Variants--
		PiEINumByDiv = RepTypeV3Detection::Has_EnableFractionals<Policy>&&Policy::EnableFractionals &&
    RepTypeV3Detection::Has_EnablePiEI<Policy>&&Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ?
    Layout::INumByDivRep : UnknownType,

		ApproachingBottomPiEI = RepTypeV3Detection::Has_EnableApproaching<Policy>&&Policy::EnableApproaching && 
    RepTypeV3Detection::Has_EnablePiEI<Policy> && Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingBottom|PiEINum : UnknownType,
		ApproachingTopPiEI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    RepTypeV3Detection::Has_EnablePiEI<Policy> && Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingTop|PiEINum : UnknownType,

		ApproachingBottomByDivPiEI = RepTypeV3Detection::EnableApproaching<Policy>&&Policy::EnableApproaching &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingBottomByDiv|PiEINum : UnknownType,
		ApproachingTopByDivPiEI = RepTypeV3Detection::Has_EnableApproachingTop<Policy>&&Policy::EnableApproachingTop &&
    Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
    Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingTopByDiv|PiEINum : UnknownType,

		ApproachingMidLeftPiEI = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingMidLeft|PiEINum : UnknownType,
		ApproachingMidRight = RepTypeV3Detection::Has_EnableApproachingDivided<Policy>&&Policy::EnableApproachingDivided &&
		Policy::EnableFractionals && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingMidRight|PiEINum : UnknownType,

		ApproachingBottomPlusOffsetPiEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingBottomPlusOffset|PiEINum : UnknownType,
		
		ApproachingTopPlusOffsetPiEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingTopPlusOffset|PiEINum : UnknownType,
		
		ApproachingBottomMinusOffsetPiEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingBottomMinusOffset|PiEINum : UnknownType,
		
		ApproachingTopMinusOffsetPiEI = RepTypeV3Detection::Has_EnableApproachingOffsets<Policy>&&Policy::EnableApproachingOffsets &&
    Policy::EnableApproaching && RepTypeV3Detection::Has_EnablePiEI<Policy> &&
		Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ApproachingTopMinusOffset|PiEINum : UnknownType,

		#pragma region Circular symbolic trig expression flags
		SinPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Sin|PiEINum : UnknownType,
		CosPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Cos|PiEINum : UnknownType,
		TanPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Tan|PiEINum : UnknownType,
		#pragma endregion Circular symbolic trig expression flags
		#pragma region Hyperbolic symbolic expression flags
		SinHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? SinH|PiEINum : UnknownType,
		CosHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? CosH|PiEINum : UnknownType,
		TanHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? TanH|PiEINum : UnknownType,
		#pragma endregion Hyperbolic symbolic expression flags
		#pragma region Inversed Circular symbolic trig expression flags(a* in math libraries usually)
		ArcSinPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcSin|PiEINum : UnknownType,
		ArcCosPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcCos|PiEINum : UnknownType,
		ArcTanPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcTan|PiEINum : UnknownType,
		#pragma endregion Inversed Circular symbolic trig expression flags(a* in math libraries usually)
    #pragma region Inversed Hyperbolic symbolic expression flags
		ArcSinHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcSinH|PiEINum : UnknownType,
		ArcCosHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcCosH|PiEINum : UnknownType,
		ArcTanHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? ArcTanH|PiEINum : UnknownType,
    #pragma endregion Inversed Hyperbolic symbolic expression flags
		#pragma region Circular reciprocal trig expression flags 
		CscPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Csc|PiEINum : UnknownType,
		SecPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Sec|PiEINum : UnknownType,
		CotPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? Cot|PiEINum : UnknownType,
		#pragma endregion Circular reciprocal trig expression flags
		#pragma region Hyperbolic reciprocal trig expression flags 
		CscHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? CscH|PiEINum : UnknownType,
		SecHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? SecH|PiEINum : UnknownType,
		CotHPiEI = RepTypeV3Detection::Has_EnableTrigExpressions<Policy>&&Policy::EnableTrigExpressions &&
		RepTypeV3Detection::Has_EnablePiEI && Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? CotH|PiEINum : UnknownType,
		#pragma endregion Hyperbolic reciprocal trig expression flags

    PiEIPower = RepTypeV3Detection::Has_EnablePowerOf<Policy>&&Policy::EnablePowerOf &&
    RepTypeV3Detection::Has_EnablePiEI<Policy>&&Policy::EnablePi && Policy::EnableE && Policy::EnableI ? ToPowerOf|PiNum|ENum|INum  : UnknownType,

    WithinMinMaxPiEIRange = RepTypeV3Detection::Has_EnableWithinMinMaxRange && Policy::EnableWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiE<Policy>&&Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? WithinMinMaxRange|PiEINum : UnknownType,
		NotWithinMinMaxPiEIRange = RepTypeV3Detection::Has_EnableNotWithinMinMaxRange && Policy::EnableNotWithinMinMaxRange &&
    RepTypeV3Detection::Has_EnablePiE<Policy>&&Policy::EnablePi&&Policy::EnableE&&Policy::EnableI ? NotWithinMinMaxRange|PiEINum : UnknownType,

		//------------------
  };
	
}