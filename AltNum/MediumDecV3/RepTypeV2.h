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

#if !defined(AltNum_UseRepTypeAsEnumOnly)
	#include "..\DLLAPI.h"
	#include <string>
#endif

namespace BlazesRusCode
{
	/*
    // RepType: Enumerates the *current* representation type of a value.
    // Describes its canonical stored form after normalisation — not the
    // sequence of operations that produced it.
		RepTypeV2 bit layout overview:

		Bits 0–3   : Core constant multipliers
								 (NormalType, PiNum, ENum, PiENum, INum)
								 → These identify the base constant family.

		Bits 4–8   : Fractional & Approaching families
								 - Bit 4: "ByDiv" — value is divided by ExtraRep (requires ExtraRep > 1)
								 - Other bits: ApproachingTop, ApproachingBottom, MidLeft, MidRight, etc.
									 Approaching forms may combine with ByDiv.

		Bits 9–13  : Infinity, PowerOf, MixedFrac, Range checks
								 → Special magnitude categories and numeric form modifiers.

	  Bits 14–15 : Fixed magnitude ± (ApproachingBottom || ApproachingTop)
	  → Describes the current stored form as a fixed-point magnitude with a
	   saved infinitesimal offset term derived from an approaching form.
	   This is a representation descriptor, not an operation history.
	
	   Fixed value (post-conversion):
	     value_fixed = IntHalf + DecimalHalf / DecimalOverflow
	   Smallest unit:
	     ε = 1 / DecimalOverflow
	     (if ByDiv is set: ε = 1 / (DecimalOverflow * ExtraRep))
	
	   Examples (using DecimalHalf explicitly):
	     2.4 + 0.00…01 → value_fixed + ε
	       → PlusOffset (Bit 14), ExtraRep sentinel = ApproachingBottom
	
	     2.4 + 0.99…9 → (IntHalf + 1) + DecimalHalf/DecimalOverflow − ε
	       → Represented as Fixed + ApproachingTop
	       → PlusOffset (Bit 14), ExtraRep sentinel = ApproachingTop
	
	   Notes:
	   - PlusOffset literally means “Fixed + saved offset term.”
	   - Values that could be re-expressed as MinusOffset are not normalized
	     ahead of time; representation reflects current stored form.
	
	   Bit 14: PlusOffset  (Fixed + ApproachingX(...))
	   Bit 15: MinusOffset (Fixed − ApproachingX(...))

		Bit 20     : IndeterminateForm
								 → Given priority over other flags; sub‑type bits may also be set.

		Bits 22–24 : Trig expression flags
								 → Symbolic trig forms (sin, cos, tan, etc.) stored in ExtraRep.

		Higher bits: Reserved for future expansion
								 (e.g., complex numbers, symbolic algebra, additional constants)
	*/
  template<class VariantName>
  enum class RepTypeV2 : uint32_t {
		NormalType = 0,
		// top bit sentinel flag
    // Reserved: highest bit in uint32_t, used only as 'unknown' sentinel.
    // No real category will be assigned to this slot.
		UnknownType = has_UnknownTypeOverride<VariantName>::value? VariantName::UnknownTypeOverride : (1u << 31);
		
    PiNum    = VariantName::EnablePi ? (1u << 1) : UnknownType,
    ENum    = VariantName::EnableE ? (1u << 2) : UnknownType,
		// Composite representation: has both Pi and E, takes precedence in type comparisons
    // Both Pi and E present → resolves to PiENum by default.
    // Still ORs Pi/E bits internally so they can be detected for multiplier logic.
    PiENum    = VariantName::EnablePiE ? PiNum|ENum : UnknownType,
		// Magnitude * imaginary number representation (can't be active at same time as PiENum at moment)
    INum    = VariantName::EnableI ? (1u << 3) : UnknownType,
		NumByDiv = VariantName::EnableFractionals ? (1u << 4) : UnknownType,
		//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
		ApproachingBottom = VariantName::EnableApproaching ? (1u << 5) : UnknownType,
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		ApproachingTop = VariantName::EnableApproachingTop ? (1u << 6) : UnknownType,
		//ApproachingBottom representation/ExtraRep
		ApproachingBottomByDiv = VariantName::EnableApproaching&&VariantName::EnableFractionals ? ApproachingBottom+NumByDiv : UnknownType,
		//ApproachingTop representation/ExtraRep
		ApproachingTopByDiv = VariantName::EnableApproachingTop&&VariantName::EnableFractionals ? ApproachingTop+NumByDiv : UnknownType,
    // Approaching midpoint from below (fractional part = 0.999… / ExtraRep)
    // Requires EnableApproachingDivided and ExtraRep divisor.
    // When ExtraRep == 1, numerically equals ApproachingTop.
		// Formula Representation: SignMultiplier * ((Magnitude of IntHalf)+(DecimalOverflow/ExtraRep)-0.0...01)
		ApproachingMidLeft = VariantName::EnableApproachingDivided ? (1u << 7) : UnknownType,
		// Formula Representation: SignMultiplier * ((Magnitude of IntHalf)+(DecimalOverflow/ExtraRep)+0.0...01)
		ApproachingMidRight = VariantName::EnableApproachingDivided ? (1u << 8) : UnknownType,
		//Sign determines if either positive or negative infinity
		Infinity = VariantName::EnableInfinity ? (1u << 9) : UnknownType,
		//Current format of AltDec only allows either ToPowerOf, MixedFrac, or (WithinMinMaxRange/NotWithinMinMaxRange) feature enabled
		//Value to power of ExtraRep
		ToPowerOf = VariantName::EnablePowerOf ? (1u << 10) : UnknownType,
		//IntValue to power of DecimalHalf/ExtraRep(Experimental Representation)(Only EnablePowerOf or EnablePowerOfV2 allowed at same time)
		ToPowerOfV2 = VariantName::EnablePowerOfV2 ? (1u << 10) : UnknownType,
		//Represents magnitude of IntHalf|DecimalHalf/ExtraRep
		MixedFrac = VariantName::EnableMixedFrac ? (1u << 11) : UnknownType,
		//Within bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		WithinMinMaxRange = VariantName::EnableWithinMinMaxRange ? (1u << 12) : UnknownType,
		//Out of bounds of IntValue(IntHalf*SignMultiplier) to (ExtraFlag?-1:1)*DecimalHalf
		NotWithinMinMaxRange = VariantName::EnableNotWithinMinMaxRange ? (1u << 13) : UnknownType,
    // Approaching offset flags:
    // These indicate an infinitesimal +ε or −ε adjustment to the base magnitude.
    // Used for special cases where the base IntHalf/DecimalHalf is fixed,
    // but approach semantics still apply.
    // Bit 14 = +ε (approach from below), Bit 15 = −ε (approach from above).
		//SignMultiplier*(NormalNum magnitude + ApproachingBottom)
		ApproachingBottomPlusOffset = VariantName::EnableApproaching&&VariantName::EnableApproachingOffsets  ? ApproachingBottom+(1u << 14) : UnknownType,
		//SignMultiplier*(NormalNum magnitude + ApproachingTop)
		ApproachingTopPlusOffset = VariantName::EnableApproachingTop&&VariantName::EnableApproachingOffsets  ? ApproachingTop+(1u << 14) : UnknownType,
		//SignMultiplier*(NormalNum magnitude - ApproachingBottom)
		ApproachingBottomMinusOffset = VariantName::EnableApproaching&&VariantName::EnableApproachingOffsets  ? ApproachingBottom+(1u << 15) : UnknownType,
		//SignMultiplier*(NormalNum magnitude - ApproachingTop)
		ApproachingTopMinusOffset = VariantName::EnableApproachingTop&&VariantName::EnableApproachingOffsets  ? ApproachingTop+(1u << 15) : UnknownType,
		
	  // --------------------
    // Indeterminate forms(Not fully implimented)
    // --------------------
    // All share the IndeterminateForm bit (bit 21).
    // Additional bits indicate the specific undefined/indeterminate case.
    // Checked first when EnableIndeterminateForms is enabled.
		IndeterminateForm = VariantName::EnableIndeterminateForms ? (1u << 21) : UnknownType,
		DividedByZero = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 1) : UnknownType,
		NaN = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 2) : UnknownType,
		Nil = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 3) : UnknownType,
		Undefined = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 4) : UnknownType,
		//0/0
		ZeroByZero = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 5) : UnknownType,
		//0 to power of 0
		ZeroToZeroPower = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 6) : UnknownType,
		//∞/∞ Representation
		//NegativeInfinityByInfinity represented if Sign bit is negative
		InfinityByInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) : UnknownType,
		InfinityMinusInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 6) : UnknownType,
		//∞^∞ Representation
		//InfinityPowerOfInfinity ∞^∞ — extreme divergence; useful to trap before overflow.
		InfinityPowerOfInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 28) : UnknownType,
		InfinityPowerOfNegativeInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 29) : UnknownType,
		//NegativeOnePowerOfInfinity represented if Sign bit is negative
		//NegativeOnePowerOfInfinity (-1)^∞ — oscillates between ±1;
		OnePowerOfInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 7) : UnknownType,
		InfinityToZeroPower = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 8): UnknownType,
		//ZeroPowerOfNegativeInfinity 0^{−∞} — diverges to ∞; distinct from ∞^∞.
		ZeroPowerOfNegativeInfinity = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 8) | (1u << 29) : UnknownType,
		UndefinedButWithinZeroInfinityRange = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 9) | (1u << 30) : UnknownType,
    //oscillatory, no limit(Given priority over later flags same as IndeterminateForm)
		//Sin(1/x) as x→0 — oscillatory, no limit if FlagState == 1
		//Cos(1/x) as x→0 — oscillatory, no limit if FlagState == 2
		//Tan(1/x) as x→0 — oscillatory, no limit if FlagState == 3
		OscillatoryNoLimit = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 10) : UnknownType,
		//OscillatoryNonTrig — non-periodic oscillations not tied to sin/cos/tan, e.g. sgn(sin(1/x)).
		OscillatoryNonTrig = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 11) : UnknownType,
		//PathDependentLimit — result depends on complex-plane approach path.
		PathDependentLimit = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 12) : UnknownType,
		//ChaoticNoLimit — bounded but aperiodic behaviour (rare, but trap‑worthy).
		ChaoticNoLimit = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 13) : UnknownType,
		//PrecisionLossUndefined — internal catastrophic cancellation or overflow kills validity.
		PrecisionLossUndefined = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 14) : UnknownType,
		//EvenRootOfNegative — e.g., √[even]{−5} over ℝ.
		EvenRootOfNegative = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 15) : UnknownType,
		//log(−x) — undefined in ℝ; valid only in ℂ.
		//Result is a potential complex number which is not currently supported by current class
		UnsupportedComplexNumberResult = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 16) : UnknownType,
		//poles of Γ(z).
		FactorialOfNegativeNonInteger  = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 17) : UnknownType,
		//e.g., arcsin(2) in R.
		InverseTrigOutOfRange = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 18) : UnknownType,
    //UnimplementedOperation — not mathematically undefined, just not yet coded.
		UnimplementedOperation = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 19) : UnknownType,
		//DomainError — generic “arg outside allowed domain” catch‑all.
		DomainError = VariantName::EnableIndeterminateForms ? IndeterminateForm | (1u << 20) : UnknownType,
		
		// Experimental symbolic trig expression flags.
		// Used to store unevaluated forms like sin(x), cos(x), tan(x) in ExtraRep.
		// Not yet fully implemented for all operations.
		Sin = VariantName::EnableTrigExpressions ? (1u << 22) : UnknownType,
		Cos = VariantName::EnableTrigExpressions ? (1u << 23) : UnknownType,
		Tan = VariantName::EnableTrigExpressions ? (1u << 24) : UnknownType,
		//--PiNum Variants--
		PiNumByDiv = VariantName::EnablePi&&VariantName::EnableFractionals ? PiNum|NumByDiv : UnknownType,
		ApproachingBottomPi = VariantName::EnablePi&&VariantName::EnableApproaching ? PiNum|ApproachingBottom : UnknownType,
		ApproachingTopPi = VariantName::EnablePi&&VariantName::EnableApproachingTop ? PiNum|ApproachingTop : UnknownType,
		ApproachingMidLeftPi = VariantName::EnablePi&&VariantName::EnableApproachingDivided ? PiNum|ApproachingMidLeft : UnknownType,
		ApproachingMidRightPi = VariantName::EnablePi&&VariantName::EnableApproachingDivided ? PiNum|ApproachingMidRight : UnknownType,
		PiPower = VariantName::EnablePi&&VariantName::EnablePowerOf ? PiNum|ToPowerOf : UnknownType,
		PiPowerV2 = VariantName::EnablePi&&VariantName::EnablePowerOfV2 ? PiNum|ToPowerOfV2 : UnknownType,
		MixedPi = VariantName::EnablePi&&VariantName::EnableMixedFrac ? PiNum|MixedFrac : UnknownType,
		SinPi = VariantName::EnablePi&&VariantName::EnableTrigExpressions ? PiNum|Sin : UnknownType,
		CosPi = VariantName::EnablePi&&VariantName::EnableTrigExpressions ? PiNum|Cos : UnknownType,
		TanPi = VariantName::EnablePi&&VariantName::EnableTrigExpressions ? PiNum|Tan : UnknownType,
		//------------------
		//--ENum Variants--
		ENumByDiv = VariantName::EnableE&&VariantName::EnableFractionals ? ENum|NumByDiv : UnknownType,
		ApproachingBottomE = VariantName::EnableE&&VariantName::EnableApproaching ? ENum|ApproachingBottom : UnknownType,
		ApproachingTopE = VariantName::EnableE&&VariantName::EnableApproachingTop ? ENum|ApproachingTop : UnknownType,
		ApproachingMidLeftE = VariantName::EnableE&&VariantName::EnableApproachingDivided ? ENum|ApproachingMidLeft : UnknownType,
		ApproachingMidRightE = VariantName::EnableE&&VariantName::EnableApproachingDivided ? ENum|ApproachingMidRight : UnknownType,
		EPower = VariantName::EnableE&&VariantName::EnablePowerOf ? ENum|ToPowerOf : UnknownType,
		EPowerV2 = VariantName::EnableE&&VariantName::EnablePowerOfV2 ? ENum|ToPowerOfV2 : UnknownType,
		MixedE = VariantName::EnableE&&VariantName::EnableMixedFrac ? ENum|MixedFrac : UnknownType,
		SinE = VariantName::EnableE&&VariantName::EnableTrigExpressions ? ENum|Sin : UnknownType,
		CosE = VariantName::EnableE&&VariantName::EnableTrigExpressions ? ENum|Cos : UnknownType,
		TanE = VariantName::EnableE&&VariantName::EnableTrigExpressions ? ENum|Tan : UnknownType,
		//------------------
		//--INum Variants--
		INumByDiv = VariantName::EnableI&&VariantName::EnableFractionals ? INum|NumByDiv : UnknownType,
		ImaginaryInfinity = VariantName::EnableI&&VariantName::EnableInfinity ? INum|Infinity : UnknownType,
		ApproachingBottomI = VariantName::EnableI&&VariantName::EnableApproaching ? INum|ApproachingBottom : UnknownType,
		ApproachingTopI = VariantName::EnableI&&VariantName::EnableApproachingTop ? INum|ApproachingTop : UnknownType,
		ApproachingMidLeftI = VariantName::EnableI&&VariantName::EnableApproachingDivided ? INum|ApproachingMidLeft : UnknownType,
		ApproachingMidRightI = VariantName::EnableI&&VariantName::EnableApproachingDivided ? INum|ApproachingMidRight : UnknownType,
		IPower = VariantName::EnableI&&VariantName::EnablePowerOf ? INum|ToPowerOf : UnknownType,
		IPowerV2 = VariantName::EnableI&&VariantName::EnablePowerOfV2 ? INum|ToPowerOfV2 : UnknownType,
		MixedI = VariantName::EnableI&&VariantName::EnableMixedFrac ? INum|MixedFrac : UnknownType,
		SinI = VariantName::EnableI&&VariantName::EnableTrigExpressions ? INum|Sin : UnknownType,
		CosI = VariantName::EnableI&&VariantName::EnableTrigExpressions ? INum|Cos : UnknownType,
		TanI = VariantName::EnableI&&VariantName::EnableTrigExpressions ? INum|Tan : UnknownType,
		//------------------
		//--PiENum Variants--
		PiENumByDiv = VariantName::EnablePiE&&VariantName::EnableFractionals ? INum|NumByDiv : UnknownType,
		ApproachingBottomPiE = VariantName::EnablePiE&&VariantName::EnableApproaching ? PiENum|ApproachingBottom : UnknownType,
		ApproachingTopPiE = VariantName::EnablePiE&&VariantName::EnableApproachingTop ? PiENum|ApproachingTop : UnknownType,
		ApproachingMidLeftPiE = VariantName::EnablePiE&&VariantName::EnableApproachingDivided ? PiENum|ApproachingMidLeft : UnknownType,
		ApproachingMidRightPiE = VariantName::EnablePiE&&VariantName::EnableApproachingDivided ? PiENum|ApproachingMidRight : UnknownType,
		PiEPower = VariantName::EnablePiE&&VariantName::EnablePowerOf ? PiENum|ToPowerOf : UnknownType,
		PiEPowerV2 = VariantName::EnablePiE&&VariantName::EnablePowerOfV2 ? PiENum|ToPowerOfV2 : UnknownType,
		MixedPiE = VariantName::EnablePiE&&VariantName::EnableMixedFrac ? PiENum|MixedFrac : UnknownType,
		SinPiE = VariantName::EnablePiE&&VariantName::EnableTrigExpressions ? PiENum|Sin : UnknownType,
		CosPiE = VariantName::EnablePiE&&VariantName::EnableTrigExpressions ? PiENum|Cos : UnknownType,
		TanPiE = VariantName::EnablePiE&&VariantName::EnableTrigExpressions ? PiENum|Tan : UnknownType
		//------------------
  };
}