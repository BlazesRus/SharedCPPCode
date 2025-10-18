// IMPORTANT REPRESENTATION NOTE:
// RestrictedFloat encodes values as:
//     value = 2^-(ExpMag + ExpFrac / MaxDenom)
// This is a FIXED-POINT exponent format:
// - 'ExpMag' is the integer exponent component.
// - 'ExpFrac' is the fractional exponent component, in ticks of 1/MaxDenom.
// - MaxDenom = 1 << FRAC_BITS.
// This is NOT a normalized mantissa+exponent float, and NOT just an inverted IEEE float.
// All arithmetic must be done in exponent-tick space, not by shifting a mantissa.
// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// Code adjusted with help of Microsoft Copilot
// ***********************************************************************
#pragma once
#include <cstdint>
#include <../RestrictedFloatBase.hpp>
#include "../PartialDec/ExpandedUDec.hpp"//For when need 0 - 18446744073709551615.999999999 range plus ExtraRep divisor
#include "../PartialDec/PartialUDec.hpp"//For when need 0-4294967295.999999999 range plus ExtraRep divisor
#include "../FloatingOperations.hpp"
#include "../OtherFunctions/VariableConversionFunctions.h"//For integer to String conversion
#include "../BlazesRusIntegerCode.hpp"//BlazesRusIntegerCode::IntPow(value,exp) and BlazesRusIntegerCode::UIntPow(value,exp)
#include <bit>      // std::bit_width
#include <cstdint>  // uint32_t, uint64_t
#include <utility>  // std::pair
#include "../FloatingOperations.hpp"//For floating power of code
#include "../PolicyConcepts.hpp"//For Policy template existance checks
#include "BinaryTypeSelector.hpp"//For selecting types based on bit total

namespace BlazesRusCode {

#if defined(__SIZEOF_INT128__)
  using u128 = unsigned __int128;   // native, fastest when available
  #define HAVE_NATIVE_UInt128 1
#else
  using u128 = boost::multiprecision::uint128_t;
  #define HAVE_NATIVE_UInt128 0
#endif
using u32  = std::uint32_t;
using u64  = std::uint64_t;

//RFStatus Defined in Base file
//RFOpResultBase Defined in Base file

struct RestrictedFloat_FalseSelector{};

struct ExperimentalFloatFeatures{
  //Experimental alternative base values other than 2 used here etc
}

//PowerOf3,5,7, 15 and mixed radix features custom toggled from here
struct RestrictedFloatExtraExperimentalFeatures {
  static inline constexpr unsigned NScaleFactor = Has_NScaleFactor ? Policy::NScaleFactor : 1;
	//Allows MaxDenom to be something other than (1u << ExpFracBits)) if this is true
  static constexpr bool UseCustomDenom = Has_UseCustomDenom ? Policy::UseCustomDenom : false;
};

struct RepTypeV3Layout {
  unsigned PiNumRep;
  unsigned ENumRep;
  unsigned INumRep;

  unsigned PiENumRep;
  unsigned PiINumRep;
  unsigned EINumRep;
  unsigned PiEINumRep;
  unsigned total;
};

namespace RepTypeV3Detection {
	template<typename Policy>
	concept Has_EnablePi   = requires { { Policy::EnablePi }   -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableE    = requires { { Policy::EnableE }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableI    = requires { { Policy::EnableI }    -> std::convertible_to<bool>; };
}

// Experiment Split Raw mode (includes optional ExtendedRange)
template<typename Policy, typename OriginalPolicy>
struct RestrictedFloatSplitRawModeExtras
{
  //Number of ExpandedRangeBits for ExtendedRange inside ExtraRaw (known at compile time)
  //Each fraction into ExtendedRange doubles the effective precision of ExpMag and ExpFrac
  static inline constexpr unsigned ExpandedRangeBits =
    Has_ExpandedRangeBits<Policy> ? Policy::ExpandedRangeBits : 0u;

  // total tick-corridor bits = base-mag+frac bits + any extra bits
  static inline constexpr unsigned ExtendedRangeTickBits =
    Policy::MagnitudeBits + ExpandedRangeBits;

  // 1) Denominator in extended mode = 1 << (FB + EB)
  //    so we need a type with at least FB+EB bits:
  using ExtendedRangeDenomT = 
    BinaryTypeSelector::TypeForBits<Policy::ExpFracBits + ExpandedRangeBits>;

  // select the smallest unsigned type that holds ExtendedRangeTickBits
  using ExtendedRangeTicksT =
  typename BinaryTypeSelector::template TypeForBits<ExtendedRangeTickBits>::type;

  static inline constexpr bool EnableExtendedRange = 
  ExpandedRangeBits>0 ? true : false;

  static inline constexpr bool EnablePi = 
  RepTypeV3Detection::Has_EnablePi<OriginalPolicy> ? true : false;
  static inline constexpr bool EnableE = 
  RepTypeV3Detection::Has_EnableE<OriginalPolicy> ? true : false;
  static inline constexpr bool EnableI = 
  RepTypeV3Detection::Has_EnableI<OriginalPolicy> ? true : false;

  //Generates RepTypeV3 layout
  static constexpr RepTypeV3Layout ComputeRepTypeLayout() noexcept {
    unsigned totalExtraFlags = 0;
  
    unsigned PiNumRep = 0, ENumRep = 0, INumRep = 0;
  
    if constexpr (EnablePi) {
      PiNumRep = 1u << totalExtraFlags++;
    }
    if constexpr (EnableE) {
      ENumRep = 1u << totalExtraFlags++;
    }
    if constexpr (EnableI) {
      INumRep = 1u << totalExtraFlags++;
    }
  
    unsigned PiENumRep   = PiNumRep | ENumRep;
    unsigned PiINumRep   = PiNumRep | INumRep;
    unsigned EINumRep    = ENumRep  | INumRep;
    unsigned PiEINumRep  = PiNumRep | ENumRep | INumRep;
  
    return RepTypeV3Layout{ PiNumRep, ENumRep, INumRep,
                            PiENumRep, PiINumRep, EINumRep, PiEINumRep,
                            totalExtraFlags };
  }
	
  static inline constexpr RestrictedFloatCode::BitLayout RepTypeLayout = ComputeRepTypeLayout();

  using RepType = RepTypeV3<OriginalPolicy, RepTypeLayout>;

  static inline constexpr unsigned ExtraFeatureBits = RepTypeLayout.total;

  static inline constexpr bool EnableExtraFeatures = ExtraFeatureBits > 0 ? true : false;

  static inline constexpr bool EnableZeroSentinal = OriginalPolicy::EnableZeroSentinal||EnableExtendedRange  ? true : false;

  static inline constexpr unsigned ExtraRawTotal = Policy::signTotal + ExpandedRangeBits + ExtraFeatureBits + (EnableZeroSentinal? 1u : 0u);

  using ExtraRawT        = BinaryTypeSelector::TypeForBits<ExtraRawTotal>;

  //Right to left layout of ExpandedRangeBits,expSignBits,signBits,ZeroSentinal
  ExtraRawT ExtraRaw;

	template<typename Policy>
	inline RepType ExtractRepType() {
		// Always project down to 32 bits, since RepTypeV3 is uint32_t‑backed
		uint32_t masked = static_cast<uint32_t>(rawFlags & static_cast<ExtraRawT>(Policy::AllowedMask));
		return static_cast<RepTypeV3>(masked);
	}

	//Bit combination used for Zero sentinal if ZeroSentinal Enabled
  static inline constexpr ExtraRawT ZeroSentinalMask = Policy::EnableZeroSentinal ? (ExtraRawT(1) << (ExtraRawTotal - 1)) : ExtraRawT(0);

	static inline constexpr ExtraRawT PositiveSign   = Has_PositiveSign ? Policy::PositiveSign : (Has_NegativeSign?!Policy::NegativeSign:1);
	static inline constexpr ExtraRawT NegativeSign   = PositiveSign ? ExtraRawT(0) : ExtraRawT(1);

  static inline constexpr ExtraRawT EXPANDED_MASK =
    (ExpandedRangeBits == 0) ? 0 : ((ExtraRawT(1) << ExpandedRangeBits) - 1);

  static inline constexpr ExtraRawT ExpSignMask =
    (Has_SignedExpMode<Policy> ? ExtraRawT(1) << ExpandedRangeBits : 0);

  static inline constexpr ExtraRawT SignMask =
    (Has_SignedMode<Policy> ? ExtraRawT(1) << (ExpandedRangeBits + expSignBits)) : 0);

  constexpr int extraBitWidth = sizeof(ExtraRawT) * 8;

  // If true, then we're in the "above fractional" band (integer lane)
  constexpr bool AboveFractionalBands(StoreT raw) const noexcept {
      if constexpr (Has_SignedExpMode<Policy>) {
          return (ExtraRaw & ExpSignMask) != 0;
      } else {
          return false;
      }
  }

};

//Separating masks to make sure that SplitRawMode has separate version of the same
template<typename Policy, typename OriginalPolicy>
struct RestrictedFloatStandaloneMasks
{
	// PositiveSign and NegativeSign are logical convention constants (1/0 or true/false).
	// They define whether a sign bit value of 1 means "positive" (this is inverted from the usual two's-complement convention).
	// The "sign bit" here refers to the logical sign indicator for this type, not necessarily a literal bit in all modes.
	// Defaults: PositiveSign = 1, NegativeSign = 0 if neither is provided in the policy.
	// Exactly one may be defined in the policy; the other is derived to be its opposite.
	// In signed mode, negative values do NOT have their magnitude bits flipped — only the sign indicator changes.
	// This avoids magnitude inversion overhead and enhances multiplication/division speed for negative values.
	static inline constexpr StoreT PositiveSign   = 
  Has_PositiveSign<OriginalPolicy> ? OriginalPolicy::PositiveSign : (Has_NegativeSign<OriginalPolicy>?!OriginalPolicy::NegativeSign:1);
	static inline constexpr Policy::StoreT NegativeSign   = OriginalPolicy::PositiveSign ? Policy::StoreT(0) : Policy::StoreT(1);
	
	static inline constexpr StoreT SignMask =
			Policy::SignedMode ? (Policy::StoreT(1) << (Policy::bitWidth - 1)) : StoreT(0);

	static inline constexpr StoreT ExpSignMask = 
  Policy::SignedExpMode ? (Policy::StoreT(1) << (Policy::bitWidth - 1 - (Policy::SignedMode? 1 : 0))) : StoreT(0);

  static inline constexpr bool EnableZeroSentinal = false;

}

template<typename Policy, typename OriginalPolicy>
struct RestrictedFloatFeatureExtensions
{
}

//This selects whether to include StandaloneMode functionality(for when RestrictedFloat is not used with a base class)
template<typename Policy, typename OriginalPolicy>
struct RestrictedFloatStandaloneSelector : std::conditional_t<Has_SplitRawMode<Policy>, RestrictedFloatSplitRawModeExtras<Policy, OriginalPolicy>, RestrictedFloatStandaloneMasks<Policy, OriginalPolicy>>,
{
protected:
  RestrictedFloatStandaloneSelector() noexcept
    : std::conditional_t<Has_SplitRawMode<Policy>, RestrictedFloatSplitRawModeExtras<Policy, OriginalPolicy>, RestrictedFloatStandaloneMasks<Policy>>{}
    {}
};

//Constexpr and members based on non-binary calculated MaxDenomStored here
template<typename Policy>
struct RestrictedFloatCustomDenomSelector : std::conditional_t<Has_UseCustomDenom<Policy>, RestrictedFloatExtraCustomDenomSupport<Policy>, RestrictedFloat_FalseSelector>,
{
protected:
  using ExtraPart = std::conditional_t<Has_UseCustomDenom,RestrictedFloatExtraCustomDenomSupport<Policy>, RestrictedFloat_FalseSelector>;
  
  RestrictedFloatCustomDenomSelector() noexcept : ExtraPart() {}
};

namespace RestrictedFloatCode {
  struct BitLayout {
    unsigned expMag;
    unsigned expFrac;
    unsigned total;
  };
}

template<class Policy = RestrictedFloat_FalseSelector>
class DLL_API RestrictedFloatPolicyOptimizer : public RestrictedFloatBase {
  static inline constexpr bool StandaloneMode =
      Has_StandaloneMode<Policy> ? Policy::StandaloneMode : false;

  static_assert(!(Has_UnsignedMode && Has_SignedMode),
  "Policy cannot define both UnsignedMode and SignedMode");

  //If not in Standalone mode, then RestrictedFloat itself is assumed to be unsigned because RestrictedFloat is acting as a TrailingDigits tail
  static inline constexpr bool SignedMode = Has_SignedMode&&StandaloneMode ? Policy::SignedMode : false;
  static inline constexpr bool SignedExpMode = Has_SignedExpMode&&StandaloneMode ? Policy::SignedExpMode : false;

  //Splits the raw into 2 different raws (main raw only includes ExpMag and ExpFrac)
  //Allows existance of scaled extended range
  static inline constexpr bool SplitRawMode =
      Has_SplitRawMode<Policy> ? Policy::SplitRawMode : false;
			
  static inline constexpr bool EnableExpandedRange =
      Has_ExpandedRangeBits<Policy> ? true : false;

  // Expansion ratio constants (constexpr, read in helpers)
  static inline constexpr unsigned ExpFracExpansionRate =
      Has_ExpFracExpansionRate<Policy> ? Policy::ExpFracExpansionRate : 5u; // 1/5 mag vs frac
  static inline constexpr unsigned ExpFracExpansionOffset = ExpFracExpansionRate - 1u;

  // Pure constexpr helpers (no external state mutated)
  static constexpr RestrictedFloatCode::BitLayout
  FillBitsToRaw(unsigned int ExpMagBits, unsigned int ExpFracBits,
                unsigned BitsToFill, unsigned BitTotal) noexcept
  {
    if constexpr (Has_ExpMagBits<Policy> && !Has_ExpFracBits<Policy>) {
      // ExpMag fixed → push all fill into frac
      ExpFracBits += BitsToFill;
    }
    else if constexpr (Has_ExpFracBits<Policy> && !Has_ExpMagBits<Policy>) {
      // ExpFrac fixed → push all fill into mag
      ExpMagBits += BitsToFill;
    }
    else {
      // Neither fixed → distribute with 1/ExpFracExpansionRate ratio
      unsigned expMagExpansion  = BitsToFill / ExpFracExpansionRate;
      unsigned expFracExpansion = BitsToFill - ExpFracExpansionRate * expMagExpansion;
      expFracExpansion += expMagExpansion * ExpFracExpansionOffset;
      ExpMagBits  += expMagExpansion;
      ExpFracBits += expFracExpansion;
    }
    return RestrictedFloatCode::BitLayout{ ExpMagBits, ExpFracBits, BitTotal };
  }

  static constexpr RestrictedFloatCode::BitLayout
  FillBitsToRawPlusComputeTotal(unsigned int ExpMagBits, unsigned int ExpFracBits,
                                unsigned BitsToFill, unsigned SignTotal) noexcept
  {
    if constexpr (Has_ExpMagBits<Policy> && !Has_ExpFracBits<Policy>) {
      ExpFracBits += BitsToFill;
    }
    else if constexpr (Has_ExpFracBits<Policy> && !Has_ExpMagBits<Policy>) {
      ExpMagBits += BitsToFill;
    }
    else {
      unsigned expMagExpansion  = BitsToFill / ExpFracExpansionRate;
      unsigned expFracExpansion = BitsToFill - ExpFracExpansionRate * expMagExpansion;
      expFracExpansion += expMagExpansion * ExpFracExpansionOffset;
      ExpMagBits  += expMagExpansion;
      ExpFracBits += expFracExpansion;
    }
    unsigned total = SignTotal + ExpFracBits + ExpMagBits;
    return RestrictedFloatCode::BitLayout{ ExpMagBits, ExpFracBits, total };
  }

  // Optimizes bit structure of the raw based on Policy to maximize raw usage
  //This only optimizes the main raw(SplitRawMode has 2 raws)
  static constexpr RestrictedFloatCode::BitLayout ComputeLayout() noexcept {
    // Sign bits are present only in Standalone mode (per your model)
    constexpr unsigned signBits    = SignedMode ? 1u : 0u;
    constexpr unsigned expSignBits = SignedExpMode ? 1u : 0u;
    constexpr unsigned signTotal   = signBits + expSignBits;
    constexpr bool allow32BitRaw   = SplitRawMode||signTotal==0u;

    // Defaults: 7/25 split
    unsigned int ExpMagBits  = Has_ExpMagBits<Policy>  ? Policy::ExpMagBits  : 7u;
    unsigned int ExpFracBits = Has_ExpFracBits<Policy> ? Policy::ExpFracBits : 25u;
    
    unsigned int BitTotal = SplitRawMode?ExpMagBits + ExpFracBits : signTotal + ExpMagBits + ExpFracBits;

    // Skip optimization if both fields are fixed by policy
    if constexpr (!Has_ExpMagBits<Policy> && !Has_ExpFracBits<Policy>) {
      // Minimum sizing:
      // - No sign bits → 32-bit minimum
      // - Either sign bit present → 64-bit minimum
      if (allow32BitRaw && BitTotal < 32u) {
        unsigned BitsToFill = 32u - BitTotal;
        return FillBitsToRaw(ExpMagBits, ExpFracBits, BitsToFill, 32u);
      }
      else if (hasAnySign && BitTotal < 64u) {
        unsigned BitsToFill = 64u - BitTotal;
        return FillBitsToRaw(ExpMagBits, ExpFracBits, BitsToFill, 64u);
      }
      else if (BitTotal > 64u) {
        // Align to 64-bit boundary if exceeding 64
        unsigned Int64BoundaryOffset = BitTotal % 64u;
        if (Int64BoundaryOffset != 0u) {
          unsigned BitsToFill = 64u - Int64BoundaryOffset;
          if constexpr (SplitRawMode)
            return FillBitsToRawPlusComputeTotal(ExpMagBits, ExpFracBits, BitsToFill, 0);
          else
            return FillBitsToRawPlusComputeTotal(ExpMagBits, ExpFracBits, BitsToFill, signTotal);
        }
      }
    }

    return RestrictedFloatCode::BitLayout{ ExpMagBits, ExpFracBits, BitTotal };
  }

  // Final resolved layout (constexpr)
  static inline constexpr RestrictedFloatCode::BitLayout Layout = ComputeLayout();

public:
  static inline constexpr unsigned ExpMagBits  = Layout.expMag;
  static inline constexpr unsigned ExpFracBits = Layout.expFrac;
  static inline constexpr unsigned BitTotal    = Layout.total;

  static inline constexpr unsigned MagnitudeBits = ExpMagBits + ExpFracBits;
  static inline constexpr unsigned IntermediateBits = MagnitudeBits*2;

  using StoreT        = BinaryTypeSelector::TypeForBits<BitTotal>;
  using IntermediateT = BinaryTypeSelector::TypeForBits<IntermediateBits>;
  using ExpMagT       = BinaryTypeSelector::TypeForBits<ExpMagBits>;
  using ExpFracT      = BinaryTypeSelector::TypeForBits<ExpFracBits>;

  using DefaultOverflowT = std::conditional_t<requires { typename Policy::DefaultOverflowT; },
                                              typename Policy::DefaultOverflowT, u64>;
  constexpr int bitWidth = sizeof(StoreT) * 8;
	
};

/// <summary>
/// Compact fractional representation used as TrailingDigits in MixedMode configurations of MediumDecV3Variant.
/// Encodes values in the range (0, ~1) as: 1 / (2^(ExpMag + ExpFrac / MaxDenom))
/// - `ExpMag` and `ExpFrac` are encoded fields; `MaxDenom` is a compile-time constant.
/// - Template parameters control precision, scaling, and layout.
/// - Default footprint: 4 bytes.
/// - zero represented by raw == ZeroRaw (Actual value depends on policy settings)
/// Primary purpose: extends precision beneath the main MixedDec value, recovering lost fractional detail.
/// Designed for deterministic, monotonic behavior across platforms.
/// Ideal for fixed-point math where sub-bit granularity is needed without floating-point drift.
/// </summary>
template<class Policy  = RestrictedFloat_FalseSelector>
class DLL_API RestrictedFloat :  RestrictedFloatPolicyOptimizer<Policy>, RestrictedFloatStandaloneSelector<RestrictedFloatPolicyOptimizer<Policy>>
{
public:													
  static inline constexpr StoreT ZeroBitsRaw = StoreT(0u);

  // Derived limits
  static inline constexpr MagnitudeT MaxDenom  = ExpFracBits == 0 ? MagnitudeT(1u) : (MagnitudeT(1u) << MagnitudeT(ExpFracBits));
  static inline constexpr MagnitudeT MaxExpFrac   = MaxDenom - MagnitudeT(1);
  static inline constexpr ExpMagT MaxExpMag   = (ExpMagBits == 0 ? 0u : ((ExpMagT(1u) << ExpMagT(ExpMagBits)) - ExpMagT(1)));

	static inline constexpr StoreT ZeroExpRep =
			StandaloneMode&&!SignedExpMode
					? (Has_ZeroExpRep ? Policy::ZeroExpRep : MaxExpMag) // default: peg exp at max in StandaloneMode mode
					: 0u;                                             // default: exp==0 in normal mode

	// Significand part of zero
	static inline constexpr StoreT ZeroExpFracRep =
			StandaloneMode&&!SignedExpMode
					? (Has_ZeroExpFracRep ? Policy::ZeroExpFracRep : MaxExpFrac)
					: 0u;

	// Now build the raw encoding for mathematical zero
  //Defined at (0,0,SubLane) or (MaxExpMag,MaxExpFrac)
	static inline constexpr StoreT ZeroRaw =
			(StoreT(ZeroExpRep) << ExpFracBits) | StoreT(ZeroExpFracRep);

  // Masks (layout: [expMag | expFrac])
  static inline constexpr StoreT EXPFRAC_MASK = (ExpFracBits == 0)
      ? StoreT{0}
      : (StoreT{1} << ExpFracBits) - StoreT{1};

  static inline constexpr StoreT EXPMAG_MASK = ~EXPFRAC_MASK;

  // Static checks

  static_assert(ExpMagBits + ExpFracBits > 0,
                "At least one of ExpMagBits or ExpFracBits must be > 0");

  // raw 32-bit storage by default
  StoreT raw;
  
  static constexpr unsigned ExpBits  = std::popcount(ExpMask);
  static constexpr unsigned ExpLSB   = std::countr_zero(ExpMask);
  static constexpr unsigned TotalBits  = sizeof(StoreT) * CHAR_BIT;

  constexpr RestrictedFloat() noexcept : raw(ZeroRaw)
	requires(!SplitRawMode)
	{}
	
  constexpr RestrictedFloat() noexcept : raw(ZeroRaw), ExtraRaw(ExtraRawT(0))
	requires(SplitRawMode&&!EnableZeroSentinal)
	{}
  constexpr RestrictedFloat() noexcept : raw(ZeroRaw), ExtraRaw(ExtraRawT(ZeroSentinalMask))
	requires(SplitRawMode&&EnableZeroSentinal)
	{}
	
  constexpr RestrictedFloat(unsigned exp, unsigned sig) noexcept 
  : raw((exp << ExpFracBits) | (sig & MaxExpFrac)) 
  {
  // if somebody passed exp==0 and sig==0, stays zero
  // if exp>MaxExpMag or sig>MaxExpFrac: caller error
  }

  constexpr RestrictedFloat(const StoreT& copy) noexcept : raw(copy) {}

  // extractors
	constexpr ExpMagT expMag() const noexcept {
			StoreT payload = raw;
			if constexpr (StandaloneMode && !SplitRawMode) {
					if constexpr (SignedMode)    payload &= ~SignMask;
					if constexpr (SignedExpMode) payload &= ~ExpSignMask;
			}
			return ExpMagT(payload >> ExpFracBits);
	}

	constexpr ExpFracT expFrac() const noexcept {
			return ExpFracT(raw & MaxExpFrac);
	}

	
  constexpr bool   isZero() const noexcept {
	  if constexpr(EnableZeroSentinal)
		  return ExtraRaw == ZeroSentinalMask;
		else
	    return raw == ZeroRaw;
	}

  // Smallest non-zero magnitude encoding
  // leave MaxExpFrac for ZeroRaw in StandaloneMode mode unless ZeroSentinal enabled
	static inline constexpr unsigned AlmostZeroSignif =
			StandaloneMode && !EnableZeroSentinal ? (MaxExpFrac - 1u) : MaxExpFrac;

	static inline constexpr unsigned AlmostZeroExp = MaxExpMag;

	// Build the raw encoding
	static inline constexpr StoreT AlmostZeroRaw =
			(StoreT(AlmostZeroExp) << ExpFracBits) | StoreT(AlmostZeroSignif);

  // ----- partial-dec shuttle -----
  static inline constexpr unsigned QuarterMaxDenom   = MaxDenom/4;
  static inline constexpr unsigned ThreeFourthMaxDenom   = QuarterMaxDenom*3;
  static inline constexpr unsigned TwiceMaxDenom   = MaxDenom*2;

	// Zero fast path
	inline void SetAsZero() noexcept {
			raw = ZeroRaw;
			if constexpr (SplitRawMode) {
					if constexpr (EnableZeroSentinal)
							ExtraRaw = ZeroSentinalMask;
					else
							ExtraRaw = ExtraRawT(0);
			}
	}
  
  // +1.0 (exp=0, signif=0, integer lane)
  static inline constexpr StoreT OneRaw =
      StandaloneMode?(StoreT(0) << ExpFracBits) : 0u;
  
  // −1.0
  static inline constexpr StoreT NegativeOneRaw =
      SignedMode?(OneRaw | SignMask) : 0u;

  //Is either positive or negative one
	constexpr bool IsOneVal() const noexcept {
    if constexpr(!StandaloneMode)
      return false;
	  else if constexpr(SplitRawMode){
      return SignedMode?(raw==OneRaw&&ExtraRaw==OneRawPt2)||(raw==NegativeOneRaw&&ExtraRaw==NegativeOneRawPt2) 
      : (raw==OneRaw&&ExtraRaw==OneRawPt2);
    } else //if constexpr(StandaloneMode){
      return SignedMode? (raw==OneRaw||raw==NegativeOneRaw) : raw==OneRaw;
    }
    return false;
	}

  //Is exactly positive one
  //Return true if (0,0,IntegerLane,PositiveSign) else return false
	constexpr bool IsOne() const noexcept {
    if constexpr(!StandaloneMode)
      return false;
	  else if constexpr(SplitRawMode){
      return raw==OneRaw&&ExtraRaw==OneRawPt2;
    } else //if constexpr(StandaloneMode){
      return raw==OneRaw;
    }
	}

  //Is exactly negative one
  //Return true if (0,0,IntegerLane,NegativeSign) else return false
	constexpr bool IsNegativeOne() const noexcept {
    if constexpr(!SignedMode)
      return false;
    else if constexpr(!StandaloneMode)
      return false;
	  else if constexpr(SplitRawMode){
      return raw==NegativeOneRaw&&ExtraRaw==NegativeOneRawPt2;
    } else //if constexpr(StandaloneMode){
      return raw==NegativeOneRaw;
    }
	}

  constexpr bool IsPositive() const noexcept {
    if constexpr (!SignedMode) {
      return true; // unsigned always positive
    } else if constexpr (SplitRawMode) {
      // In SplitRawMode, sign is in ExtraRaw
      const bool bitSet = (ExtraRaw & SignMask) != 0;
      return (bitSet ? (PositiveSign != 0) : (NegativeSign != 0));
    } else {
      // StandaloneMode: sign is in raw
      const bool bitSet = (raw & SignMask) != 0;
      return (bitSet ? (PositiveSign != 0) : (NegativeSign != 0));
    }
  }


  constexpr bool IsNegative() const noexcept {
    if constexpr (!SignedMode) {
      return false;
    } else if constexpr (SplitRawMode) {
      const bool bitSet = (ExtraRaw & SignMask) != 0;
      return (bitSet ? (NegativeSign != 0) : (PositiveSign != 0));
    } else {
      const bool bitSet = (raw & SignMask) != 0;
      return (bitSet ? (NegativeSign != 0) : (PositiveSign != 0));
    }
  }

  // If true, then we're in the "above fractional" band (integer lane)
  constexpr bool AboveFractionalBands() const noexcept
  {
    if constexpr (!SignedExpMode)
        return false;
    else if constexpr (SplitRawMode) {
        return (ExtraRaw & ExpSignMask) != 0;
    else // StandaloneMode
        return (raw & ExpSignMask) != 0;
  }

	// Set only exp; signif preserved unless clamped by policy.
	inline void SetExp(unsigned newExp) noexcept {
			unsigned e = (newExp > MaxExpMag) ? MaxExpMag : newExp;
			unsigned s = expFrac(); // current

			raw = (StoreT(e) << ExpFracBits) | StoreT(s & MaxExpFrac);
	}

	// Set only signif; exp preserved, but may be clamped by policy at MAX.
	inline void SetExpFrac(unsigned newSignif) noexcept {
			unsigned e = expMag(); // current
			unsigned s = newSignif & MaxExpFrac;

			raw = (StoreT(e) << ExpFracBits) | StoreT(s);
	}

	// Generic setter: preserves sign/exp-sign if policy says so
	inline void SetMagnitudes(unsigned newExp, unsigned newSignif) noexcept {
			// Clamp
			if (newSignif > MaxExpFrac) {
					newSignif -= MaxExpFrac;
					++newExp;
			}
			if (newExp > MaxExpMag) { raw = AlmostZeroRaw; return; }
			//Don't set to Zero unless explicitly set to zero
			if constexpr (!EnableZeroSentinal){
			  if (newExp == ZeroExpRep && newSignif == ZeroExpFracRep) { raw = AlmostZeroRaw; return; }
      }
			
			StoreT payload = (StoreT(newExp) << ExpFracBits) | StoreT(newSignif);

			if constexpr (StandaloneMode&&!SplitRawMode) {
					if constexpr (SignedExpMode) {
							if constexpr (SignedMode) {
									raw = (raw & SignMask) | (raw & ExpSignMask) | payload;
							} else {
									raw = (raw & ExpSignMask) | payload;
							}
					} else if constexpr (SignedMode) {
							raw = (raw & SignMask) | payload;
					} else {
							raw = payload;
					}
			} else {
					raw = payload;
			}
	}

	// Above-fractional lane: force exp-sign bit = 1
	template<typename = std::enable_if_t<StandaloneMode && SignedExpMode>>
	inline void SetMagnitudesAboveFractional(unsigned newExp, unsigned newSignif) noexcept {
			if (newExp > MaxExpMag)
					throw("RestrictedFloat Overflow Exception");

			StoreT payload = (StoreT(newExp) << ExpFracBits) | StoreT(newSignif);

			if constexpr (SignedMode) {
					raw = (raw & SignMask) | ExpSignMask | payload;
			} else {
					raw = ExpSignMask | payload;
			}
	}

	// Fractional lane: force exp-sign bit = 0
	template<typename = std::enable_if_t<StandaloneMode>>
	inline void SetMagnitudesAtFractional(unsigned newExp, unsigned newSignif) noexcept {
			// Clamp
			if (newSignif > MaxExpFrac) {
					newSignif -= MaxExpFrac;
					++newExp;
			}
			if (newExp > MaxExpMag) { raw = AlmostZeroRaw; return; }
			//Don't set to Zero unless explicitly set to zero
			if constexpr (!EnableZeroSentinal){
			  if (newExp == ZeroExpRep && newSignif == ZeroExpFracRep) { raw = AlmostZeroRaw; return; }
      }
			
			StoreT payload = (StoreT(newExp) << ExpFracBits) | StoreT(newSignif);

			if constexpr (SignedMode) {
					raw = (raw & SignMask) | (payload & ~ExpSignMask);
			} else {
					raw = payload & ~ExpSignMask;
			}
	}

	// Standalone-only unified setter.
	// - aboveFractional: true → force exp-sign bit = 1 (integer lane), false → force exp-sign bit = 0 (fractional lane)
	// - isPositive: desired logical sign; respects PositiveSign/NegativeSign convention.
	// Preserves nothing implicitly; builds the new raw from explicit parts.
	template<typename = std::enable_if_t<StandaloneMode>>
	inline void SetStandaloneMagnitudes(unsigned newExp, unsigned newSignif,
																			bool aboveFractional = false,
																			bool isPositive      = true) noexcept
	{
			// Clamp and zero handling
			if (newSignif > MaxExpFrac) {
					newSignif -= MaxExpFrac;
					++newExp;
			}
			if (newExp > MaxExpMag) { raw = AlmostZeroRaw; return; }
			//Don't set to Zero unless explicitly set to zero
			if constexpr (!EnableZeroSentinal){
			  if (newExp == ZeroExpRep && newSignif == ZeroExpFracRep) { raw = AlmostZeroRaw; return; }
			}

			// Build payload
			StoreT payload = (StoreT(newExp) << ExpFracBits) | StoreT(newSignif);

			// Force lane bit
			if constexpr (SignedExpMode) {
					if (aboveFractional) {
							payload |= ExpSignMask;          // force exp-sign = 1
					} else {
							payload &= ~ExpSignMask;         // force exp-sign = 0
					}
			} else {
					// No exp-sign in this policy; just use payload
			}

			// Apply sign according to convention
			if constexpr (SignedMode) {
					// Compute desired sign-bit state given policy's logical convention
					const bool wantBitSet = isPositive ? (PositiveSign != 0) : (NegativeSign != 0);
					if (wantBitSet) {
							raw = payload | SignMask;        // set sign bit = 1
					} else {
							raw = payload & ~SignMask;       // set sign bit = 0
					}
			} else {
					raw = payload;                        // no sign bit in this policy
			}
	}

public:

    static inline std::string ZeroBandToString(uint32_t signifNum,
                         uint32_t fracBits,
                         uint32_t P) {
    u64 denomMax = u64{1} << fracBits;
    u64 num    = (denomMax << 1) - signifNum; // (2 * MaxDenom) - signif
    uint32_t shift = fracBits + 1;        // zero-band denominator power

    const unsigned powBits = POW10_BITLEN[P];

    // u32 fast path
    if (P < POW10_U32.size() && mul_fits_bits<32>(num, powBits)) {
      u32 scaled = u32((u64(num) * u64(POW10_U32[P])) >> shift);
      return to_dec_string(scaled);
    }
    // u64 fast path
    if (P < POW10_U64.size() && mul_fits_bits<64>(num, powBits)) {
      u64 scaled = (u64(num) * POW10_U64[P]) >> shift;
      return to_dec_string(scaled);
    }
    // wide path
    u128 scaled = (u128(num) * POW10_U128[P]) >> shift;
    return to_dec_string(scaled);
  }

  static inline std::string ToStringImpl(uint32_t exp,
                       uint32_t signifNum,
                       uint32_t fracBits,
                       uint32_t P) {
    u64 denomMax = u64{1} << fracBits;
    u64 num    = (denomMax << 1) - signifNum;
    uint32_t shift = fracBits + exp + 1; // denominator power-of-two

    const unsigned powBits = POW10_BITLEN[P];

    // u32 fast path
    if (P < POW10_U32.size() && mul_fits_bits<32>(num, powBits)) {
      u32 scaled = u32((u64(num) * u64(POW10_U32[P])) >> shift);
      return to_dec_string(scaled);
    }
    // u64 fast path
    if (P < POW10_U64.size() && mul_fits_bits<64>(num, powBits)) {
      u64 scaled = (u64(num) * POW10_U64[P]) >> shift;
      return to_dec_string(scaled);
    }
    // wide path
    u128 scaled = (u128(num) * POW10_U128[P]) >> shift;
    return to_dec_string(scaled);
  }

  /// <summary>
  /// Converts to string(Outputs string in digit display format).
  /// </summary>
  /// <returns>std.string</returns>
  std::string ToString(size_t maxDigits = 10) const {
    auto e = expMag();
    auto s = expFrac();
    // Case: Exp=0, Signif=0 -> exact zero
    if (e == 0 && expFrac() == 0)
      return "0";

    // Zero band (Exp=0, Signif != 0)
    if (e == 0)
      return ZeroBandToString(s, fracBits, static_cast<u32>(maxDigits));

    // General bands: normal (<MaxExpMag) + subnormal (>=MaxExpMag)
    return ToStringImpl(exp, s, fracBits, static_cast<u32>(maxDigits));
  }

  // Sign-aware operation dispatch for MixedDec (Exp, SignifNum noted in comments).
  // - `Op*` prefix: in-place mutation of this object.
  // - `By*` prefix: returns a modified copy, leaving this object unchanged.
  // All operations respect the current sign convention and MixedMode layout.
  // Used for arithmetic that must account for Exp/SignifNum fields and TrailingDigits precision.

  #pragma region Floating Conversion

  // Policy-aware limits using BlazesFloatingCode helpers
  static constexpr double MaxValue =
      StandaloneMode
          ? 1.0
          : 1.0 / BlazesFloatingCode::NthRootV3(2.0, MaxDenom);
  
  static constexpr double MinValue =
      (1.0 / BlazesFloatingCode::UIntPowFP(2.0, MaxExp)) /
      BlazesFloatingCode::NthRootV3(2.0, MaxDenom / (MaxDenom - 1));
  
  constexpr float ToFloat(const RTable<ExpFracBits>& rt) const noexcept {
      uq64 fracQ = rt.pow2_neg_frac(signif);
      uq64 valQ  = (exp >= Q) ? (fracQ >> exp) : (fracQ << (Q - exp));
      return static_cast<float>(valQ) / static_cast<float>(Q_ONE);
  }
  
  static constexpr RestrictedFloat FromFloat(float value, const RTable<ExpFracBits>& rt) {
      // Reject non-positive
      if (value <= 0.0f) return {};
  
      // Clamp to representable range
      if (value > static_cast<float>(MaxValue)) value = static_cast<float>(MaxValue);
      if (value < static_cast<float>(MinValue)) value = static_cast<float>(MinValue);
  
      int e;
      float frac = std::frexp(value, &e); // value = frac * 2^e, frac in [0.5,1)
      unsigned signif = rt.frac_index_from_Q(static_cast<uq64>(frac * Q_ONE));
      unsigned exp    = static_cast<unsigned>(-e); // inverted form
      return RestrictedFloat{signif, exp};
  }
  
  constexpr double ToDouble(const RTable<ExpFracBits>& rt) const noexcept {
      uq64 fracQ = rt.pow2_neg_frac(signif);
      uq64 valQ  = (exp >= Q) ? (fracQ >> exp) : (fracQ << (Q - exp));
      return static_cast<double>(valQ) / double(Q_ONE);
  }
  
  static constexpr RestrictedFloat FromDouble(double value, const RTable<ExpFracBits>& rt) {
      // Reject non-positive
      if (value <= 0.0) return {};
  
      // Clamp to representable range
      if (value > MaxValue) value = MaxValue;
      if (value < MinValue) value = MinValue;
  
      int e;
      double frac = std::frexp(value, &e);
      unsigned signif = rt.frac_index_from_Q(static_cast<uq64>(frac * Q_ONE));
      unsigned exp    = static_cast<unsigned>(-e);
      return RestrictedFloat{signif, exp};
  }

  // Explicit conversions to floating point
  explicit operator float() const noexcept {
      return ToFloat(rtable_instance); // or your direct math version
  }
  explicit operator double() const noexcept {
      return ToDouble(rtable_instance);
  }

  // Explicit constructors from floating point
  explicit RestrictedFloat(float value) {
      *this = FromFloat(value, rtable_instance);
  }
  explicit RestrictedFloat(double value) {
      *this = FromDouble(value, rtable_instance);
  }

  // No integer constructor — prevents accidental misuse
  RestrictedFloat(int) = delete;
  RestrictedFloat(unsigned) = delete;
  RestrictedFloat& operator++() = delete;
  RestrictedFloat& operator--() = delete;
  RestrictedFloat operator++(int) = delete;
  RestrictedFloat operator--(int) = delete;

  #pragma endregion Floating Conversion

  #pragma region ValueDefines
protected:
  static RestrictedFloat ZeroValue()
  {
    return RestrictedFloat();
  }
  
  /// <summary>
  /// Returns the value at 0.5
  /// </summary>
  /// <returns>RestrictedFloat</returns>
  static RestrictedFloat Point5Value()
  {
    return RestrictedFloat(0, 1);
  }
  
  /// <summary>
  /// Returns the value at 0.5
  /// </summary>
  /// <returns>RestrictedFloat</returns>
  static RestrictedFloat Point25Value()
  {
    return RestrictedFloat(0, 2);
  }
  
   /// <summary>
   /// Returns the value at zero
   /// </summary>
   /// <returns>RestrictedFloat</returns>
   static const RestrictedFloat Zero;

  #pragma endregion ValueDefines

  #pragma region Comparison Operators
  
  //Reverse ordering as exp and signif gets larger, the number gets smaller
  std::strong_ordering operator<=>(const RestrictedFloat& that) const
  {
  if (isZero()) {
    if (that.isZero()) return 0 <=> 0;
    else return 0 <=> 1;
  }
  if (that.isZero()) return 1 <=> 0;
  return std::tie(that.exp, that.signif) <=> std::tie(exp, signif);
  }
  
  bool operator==(const RestrictedFloat& that) const
  {
  if (expFrac()!=that.expFrac())
    return false;
  if (expMag()!=that.expMag())
    return false;
  return true;
  }
  
  auto operator<=>(const int& that) const
  {
  if(that>0)//Right is greater if that is above zero
    return 0<=>1;
  else if(isZero())
    return 0<=>0;
  else
    return 1<=>0;
  }

  bool operator==(const int& that) const
  {
  if(isZero()&&that==0)
    return true;
  else
    return false;
  }

  #pragma endregion Comparison Operators

  #pragma region String Commands

    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    void ReadString(const std::string& Value)
    {
      // Strip non-digits(don't need to track . (meant to be part of fixed decimal tail))
      u128 v = 0;
      u32  digits = 0;
      for (char c : Value) {
      if (c == '.') { continue; }
      if (c >= '0' && c <= '9') {
        v = v * 10 + (c - '0');
        if (!(digits==0 && c=='0')) ++digits;
      }
      }

      // Empty or zero => zero sentinel
      if (v == 0 || digits == 0) { raw = ZeroRaw; return; }

      // scale = 10^digits
      if (digits >= POW10_U128.size()) digits = static_cast<u32>(POW10_U128.size()-1);
      u128 scale = POW10_U128[digits];

      // Target value x = v / scale in (0,1). Choose band:
      // If x >= 1/2, prefer zero band for best resolution; else general band.
      // Zero band: x = 1 - s/(2D) => s = round( (1 - x) * 2D )
      const u128 D   = u128{1} << ExpFracBits;
      const u128 twoD  = D << 1;

      auto set_zero_band = [&](u128 numer, u128 denom) {
      // s = round( (1 - numer/denom) * 2D )
      u128 t = (denom > numer) ? (denom - numer) : u128{0};
      u128 s128 = (t * twoD + (denom>>1)) / denom; // round half-up
      if (s128 == 0) s128 = 1;           // avoid reserved zero sentinel in zero band
      if (s128 > twoD - 1) s128 = twoD - 1;    // clamp
      raw = (StoreT(0) << ExpFracBits) | StoreT(s128 & EXPFRAC_MASK);
      };

      auto set_general_band = [&](u128 numer, u128 denom) {
      // We want x = v/scale ≈ (2D - s) / (D * 2^(exp+1))
      // => (2D - s) ≈ x * D * 2^(exp+1) = (numer * D << (exp+1)) / denom
      // Pick exp so that (2D - s) is in [1,2D] and s in [0,2D-1].
      // Start with exp = 0 and grow until within range or capped.
      u32 e = 0;
      u128 target = 0;
      while (e < MaxExpMag) {
        u128 scaled = ( (numer * D) << (e + 1) ) / denom; // floor
        if (scaled == 0) { ++e; continue; }
        if (scaled <= twoD) { target = scaled; break; }
        ++e;
      }
      if (target == 0) {
        // Too small even at MaxExpMag -> underflow to zero(only when reading from string not in normal operation)
        raw = ZeroRaw;
        return;
      }
      // s = 2D - target
      u128 s128 = twoD - target;
      if (s128 > twoD - 1) s128 = twoD - 1; // clamp
      raw = (StoreT(e) << ExpFracBits) | StoreT( static_cast<StoreT>(s128) & EXPFRAC_MASK );
      };

      // Compare v/scale with 1/2
      if ((v << 1) >= scale) {
      set_zero_band(v, scale);
      } else {
      set_general_band(v, scale);
      }
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumUDec"/> class from string literal
    /// </summary>
    /// <param name="strVal">The value.</param>
    RestrictedFloat(const char* strVal){ ReadString(Value); }

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    RestrictedFloat(const std::string& Value){ ReadString(Value); }

public:

    /// <summary>
    /// Implements the operator std::string operator.
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator std::string() { return ToString(); }

  #pragma region String Commands

  #pragma region ExtendedRange specific
	//requires(SplitRawMode)
	

	
  #pragma endregion ExtendedRange specific


  // Sign-aware operation dispatch for MixedDec (Exp, SignifNum noted in comments).
  // - `Op*` prefix: in-place mutation of this object.
  // - `By*` prefix: returns a modified copy, leaving this object unchanged.
  // All operations respect the current sign convention and MixedMode layout.
  // Used for arithmetic that must account for Exp/SignifNum fields and TrailingDigits precision.
  // DivOp and MultOp functions unlikely to be used by the MixedDec operators
  // While AddOp and SubOp likely to be used by MixedDec
  // 
  #pragma region MixedDec Operator Helpers

  // Converts an unsigned integer magnitude into (integer exponent, fractional exponent ticks)
  // for the RestrictedFloat fixed-point exponent model:
  //   value = 2^(E + S/MaxDenom)
  // where:
  //   E = integer exponent (floor(log2(val)))
  //   S = fractional exponent in ticks [0, MaxDenom)
  // This is NOT a mantissa; both E and S are parts of the exponent itself.
  template<UnsignedIntegerType IntType = unsigned int>
  constexpr std::pair<unsigned, unsigned>
  UnpackToExpFrac(IntType val) noexcept {
      // Integer exponent = floor(log2(val))
      unsigned exp = std::bit_width(val) - 1;
  
      // Remainder after removing the top bit
      IntType remainder = val - (IntType(1) << exp);
  
      // Fractional exponent in ticks (scaled to ExpFracBits)
      unsigned fracTicks = unsigned((remainder << ExpFracBits) >> exp);
  
      // Carry into integer exponent if fractional part overflows
      if (fracTicks == (1u << ExpFracBits)) {
          fracTicks = 0;
          ++exp;
      }
  
      return {exp, fracTicks};
  }

	constexpr std::pair<NormalizedT, NormalizedT>
	NormalizePair(ExpMagT mag, ExpFracT frac) noexcept {
	  //ExpandedRange==1 starts mixed fraction exponent at (1<<ExpMag) with additional mixed fraction exponent scaled in half
    //ExpandedRange==2 starts mixed fraction exponent at (1<<ExpMag)+(1<<ExpMag)>>1) with additional mixed fraction exponent divided by 4
    //ExpandedRange==3 starts mixed fraction exponent at (1<<ExpMag)+((1<<ExpMag)>>1)+((1<<ExpMag)>>2) with additional mixed fraction exponent divided by 8
	 if constexpr (EnableExpandedRange){
	   NormalizedT ShiftedMagBits = (1<<ExpMagBits);
		 NormalizedT EffectiveMag = NormalizedT(expMag());
		 NormalizedT EffectiveFrac = NormalizedT(expFrac());
		 ExtraRawT expandedRange = ExpandedRange();
		 if(expandedRange>1){
		   //Remainder bits handled outside of this method
       // Divide magnitude into range buckets
			NormalizedT rangeDiv = NormalizedT(expandedRange + 1);
			NormalizedT magDivRes = EffectiveMag / rangeDiv;
			NormalizedT remRes    = EffectiveMag - NormalizedT(expandedRange) * magDivRes;

			remRes *= MaxDenom;
			remRes /= rangeDiv;

			EffectiveMag  = magDivRes;
			EffectiveFrac = (EffectiveFrac / rangeDiv) + remRes;

			if (EffectiveFrac >= MaxDenom) {
					EffectiveFrac -= MaxDenom;
					++EffectiveMag;
			}

			// Add geometric offset: (1<<ExpMag) + (1<<ExpMag)>>1 + ...
			for (ExtraRawT r = expandedRange; r > 0;) {
					EffectiveMag += ShiftedMagBits >> (r--);
			}
		}
		return { EffectiveMag, EffectiveFrac };
	   return { EffectiveMag, EffectiveFrac }
	 } else
	   return { NormalizedT(mag), NormalizedT(frac) }
	}


  constexpr std::pair<NormalizedT, NormalizedT>
  NormalizePair() noexcept {
	  //ExpandedRange==1 starts mixed fraction exponent at (1<<ExpMag) with additional mixed fraction exponent scaled in half
    //ExpandedRange==2 starts mixed fraction exponent at (1<<ExpMag)+(1<<ExpMag)>>1) with additional mixed fraction exponent divided by 4
    //ExpandedRange==3 starts mixed fraction exponent at (1<<ExpMag)+((1<<ExpMag)>>1)+((1<<ExpMag)>>2) with additional mixed fraction exponent divided by 8
	 if constexpr (EnableExpandedRange){
	   return NormalizePair(expMag(), expFrac());
	 } else
	   return { NormalizedT(expMag()), NormalizedT(expFrac()) }
	}

  constexpr std::pair<ExpMagT, ExpFracT>
  ExtractPair() noexcept {
	  return { expMag(), expFrac() }
	}

// MULT/DIV REMINDER:
// Multiplication/division is just integer addition/subtraction of exponent ticks or mixed fraction addition:
//   ticks = ExpMag * MaxDenom + ExpFrac
//   FullExponent = ExpMag + ExpFrac/MaxDenom
//   mul: ticks += rhs_ticks
//   div: ticks -= rhs_ticks
// No mantissa multiply is needed — the ExpFrac is part of the exponent.

	//Treating as 1/2^(this->expMag()+this->expFrac()/MaxDenom) * 1/(2^tempExp+tempSignif/MaxDenom)
  template<UnsignedIntegerType IntType=unsigned int, typename OverflowT=IntType>
  inline void UIntDivOp(const IntType& rhs){
	  auto [tempExp, tempSignif] = UnpackToExpFrac(rhs);
	  tempSignif += expFrac();
		if(tempSignif>MaxDenom){
		  tempSignif -= MaxDenom;
			++tempExp;
		}
		tempExp += expMag();
		SetMagnitudes(tempExp, tempSignif);
		//No danger of overflow with integer division
  }

  inline constexpr uint32_t pack_U(unsigned E, unsigned S) { return E*MaxDenom + S; }
  inline constexpr void unpack_U(uint32_t U, unsigned& E, unsigned& S) {
    E = U / MaxDenom; S = U % MaxDenom;
  }

  // Convert exponent ticks Δ → MediumDec ratio r = k + f/MaxDenom
  inline MediumUDec delta_ticks_to_ratio(unsigned deltaTicks) {
      unsigned k = deltaTicks / MaxDenom;
      unsigned f = deltaTicks % MaxDenom;
      // r = k + f/MaxDenom
      MediumUDec frac_u(f, 0); // f/MaxDenom as unsigned
			frac_u /= MaxDenom;//Result will be less than one
      return MediumUDec(k, frac_u.DecimalHalf);
  }

  inline MediumDec pow2_neg(const MediumDec& r) {
      // LN2 = 2^(-r) = Exp(-Ln(2) * r)
      return MediumDecExponental::Exp<MediumDec>( MediumDec::NegLn2.MultipliedBy(r) );
  }

  //Convert from numerator/denom (with PowerOfTwo denominator such as when both signif==0) back to RestrictedFloat
  template<typename TickT>
  inline void ConvertPureExpPair(TickT numerator, TickT denominator)
  {
      constexpr TickT OV = RestrictedFloat::DENOM_MAX; // MaxDenom

      // E_total = log2(denominator) - log2(numerator)
      // Whole steps:
      uint32_t denomExp = std::countr_zero(denominator);
      uint32_t numExp   = std::countr_zero(numerator);
      int32_t  expPart  = int32_t(denomExp) - int32_t(numExp);

      // Remove whole steps from denom/num
      TickT denomRem = denominator >> denomExp;
      TickT numRem   = numerator   >> numExp;

      // Fractional ticks: ratio denomRem/numRem is a power-of-two within one coarse step
      uint32_t FracPart = 0;
      if (denomRem != numRem) {
          // fracPow = log2(denomRem) - log2(numRem)
          int fracPow = int(std::countr_zero(denomRem)) - int(std::countr_zero(numRem));
          FracPart  = uint32_t(fracPow)*OV;
      }

			SetMagnitudes(uint32_t(expPart), FracPart);
  }
  
  // For the fractional-exponent band: convert from an exact power-of-two ratio
  // numerator/denominator back to (exp, signif) where
  // value = 1 / 2^(ExpMag + ExpFrac / MaxDenom)
  template<typename TickT = StoreT>
  inline void ConvertFracPair(TickT numerator, TickT denominator)
  {
      constexpr TickT OV = MaxDenom; // fractional ticks per whole exponent step

      TickT ratioNum = denominator;
      TickT ratioDen = numerator;

      int32_t expPart = 0;
      while (ratioNum >= (ratioDen << 1)) { ratioNum >>= 1; ++expPart; }
      while (ratioNum < ratioDen)         { ratioNum <<= 1; --expPart; }

      uint32_t FracPart = 0;
      if (ratioNum != ratioDen) {
          int fracPow = int(std::countr_zero(ratioNum)) - int(std::countr_zero(ratioDen));
          FracPart  = uint32_t(fracPow) * OV;
      }

			SetMagnitudes(uint32_t(expPart), FracPart);
  }

  template<UnsignedIntegerType IntType=unsigned int, typename OverflowT=IntType>
  inline void UIntMultOp(const IntType& rhs)
	requires(SignedExpMode)
	{
    // classic form
	  auto [rhsExp, rhsFrac] = UnpackToExpFrac(rhs);
		//Inverted form
		auto [lhsExp, lhsFrac] = ExtractPair();
		if(AboveFractionalBands()){
			if constexpr (EnableExpandedRange){
			  auto [normalizedLhsExp, normalizedLhsFrac] = NomalizePair();
			  //ToDo:Add Code
			} else {
			  lhsExp += rhsExp;
			  lhsFrac += lhsFrac;
			  if (lhsFrac >= MaxDenom) { lhsFrac -= MaxDenom; ++lhsExp; }
			  SetMagnitudesAboveFractional(lhsExp, lhsFrac);
			}
		} else {
			if constexpr (EnableExpandedRange){
			  auto [normalizedLhsExp, normalizedLhsFrac] = NomalizePair();
			  //ToDo:Add Code
			} else if(rhsExp>=lhsExp){
				//If rhs magnitude is equal or greater than the inverted form from lhs, then value has overflowed
				if(rhsFrac>lhsFrac){
					// Reusing rhsExp/rhsFrac as relative magnitude delta after multiply
					rhsExp -= lhsExp;// exponent delta
					rhsFrac -= lhsFrac;// fractional delta
					SetMagnitudesAboveFractional(lhsExp, lhsFrac);
				} else if(rhsFrac==lhsFrac){
					//Overflow detected but code path is simplified because no signif or TrailingDigits left
					rhsExp -= lhsExp;// exponent delta
					SetMagnitudesAboveFractional(rhsExp, 0);
				} else {
					//No Overflow Detected(Values reused for result)
					lhsExp -= rhsExp;// exponent delta
					lhsFrac -= rhsFrac;// fractional delta
					SetMagnitudes(lhsExp, lhsFrac);
				}
			} else {//No Overflow Detected
				if (lhsFrac >= rhsFrac) {
					// No borrow from exponent
					SetMagnitudes(lhsExp, lhsFrac - rhsFrac);
				} else {
					// Borrow 1 from exponent: fractional = MaxDenom - (rhsFrac - lhsFrac)
					const IntType gap = rhsFrac - lhsFrac; // 1..MaxDenom-1
					SetMagnitudes(lhsExp - 1, MaxDenom - gap);
				}
			}
		}
	}

	//Treating as 1/2^(this->expMag()+this->expFrac()/MaxDenom) * (2^tempExp+tempFrac/MaxDenom)
  template<UnsignedIntegerType IntType=unsigned int, typename OverflowT=IntType>
  inline OverflowT UIntMultOp(const IntType& rhs)
	requires(!SignedExpMode)
	{
    // classic form
	  auto [rhsExp, rhsFrac] = UnpackToExpFrac(rhs);
		//Inverted form
		auto [lhsExp, lhsFrac] = NormalizePair();
    //Both are reused for result magnitude 
		if constexpr (EnableExpandedRange){
			  //ToDo:Add Code
    } else if(rhsExp>=lhsExp){
		  //If rhs magnitude is equal or greater than the inverted form from lhs, then value has overflowed
			if(rhsFrac>lhsFrac){
        // Reusing rhsExp/rhsFrac as relative magnitude delta after multiply
        rhsExp -= lhsExp;// exponent delta
        rhsFrac -= lhsFrac;// fractional delta
				
				//To-Do:This overflow block needs updating to correct formula
      } else if(rhsFrac==lhsFrac){
        //Overflow detected but code path is simplified because no signif or TrailingDigits left
        rhsExp -= lhsExp;// exponent delta
				if constexpr (StandaloneMode && SignedExpMode){
				  SetMagnitudesAboveFractional(rhsExp, 0);
				} else {
          SetAsZero();
          return IntType(1) << rhsExp;
				}
      } else {
        //No Overflow Detected(Values reused for result)
        lhsExp -= rhsExp;// exponent delta
        lhsFrac -= rhsFrac;// fractional delta
        SetMagnitudes(lhsExp, lhsFrac);
      }
    } else {//No Overflow Detected
			if constexpr (EnableExpandedRange){
			  //ToDo:Add Code
			
      } else if (lhsFrac >= rhsFrac) {
        // No borrow from exponent
        SetMagnitudes(lhsExp, lhsFrac - rhsFrac);
      } else {
        // Borrow 1 from exponent: fractional = MaxDenom - (rhsFrac - lhsFrac)
        const IntType gap = rhsFrac - lhsFrac; // 1..MaxDenom-1
        SetMagnitudes(lhsExp - 1, MaxDenom - gap);
      }
    }
		return 0u;
  }

  template<typename OverflowT=DefaultOverflowT>
  inline OverflowT DivOp(const RestrictedFloat &rhs){
    auto lhsExp    = expMag();
    auto lhsFrac = expFrac();
    auto rhsExp    = rhs.expMag();
    auto rhsFrac = rhs.expFrac();

    if (lhsExp >= rhsExp) {
        if (lhsFrac > rhsFrac) {
            // delta = (lhsExp - rhsExp, lhsFrac - rhsFrac)
            lhsExp    -= rhsExp;    // exponent delta
            lhsFrac -= rhsFrac; 
            const IntType scale = IntType(1) << lhsExp;
            OverflowT carry = OverflowT(scale);
            const OverflowT mixed = OverflowT(scale) * OverflowT(lhsFrac);
            carry += mixed / MaxDenom;
            const IntType rem = static_cast<IntType>(mixed % MaxDenom);
            if (rem == 0) SetAsZero();
            else SetMagnitudes(0, MaxDenom - rem);
            return carry;
        }
        else if (lhsFrac == rhsFrac) {
            lhsExp -= rhsExp;
            SetAsZero();
            return IntType(1) << lhsExp;
        }
        else {
            lhsExp    -= rhsExp;
            rhsFrac -= lhsFrac; // borrow-safe if you normalise
            SetMagnitudes(lhsExp, rhsFrac);
        }
    } else {
        // no overflow, borrow-safe subtraction
        rhsExp -= lhsExp;
        if (rhsFrac >= lhsFrac) {
            SetMagnitudes(rhsExp, rhsFrac - lhsFrac);
        } else {
            const IntType gap = lhsFrac - rhsFrac;
            SetMagnitudes(rhsExp - 1, MaxDenom - gap);
        }
    }
    return OverflowT(0);
  }
  
	template<typename StoreT=DefaultOverflowT>
  inline void MultOp(const RestrictedFloat &rhs){
    auto sumExp    = expMag()    + rhs.expMag();
    auto sumFrac = expFrac() + rhs.expFrac();
    if (sumFrac >= MaxDenom) { sumFrac -= MaxDenom; ++sumExp; }
    SetMagnitudes(sumExp, sumFrac);
  }

// ADD/SUB REMINDER:
// Because this is a fixed-point exponent representation, addition/subtraction
// must adjust the exponent term by log2(1 ± 2^-Δ) where Δ is the exponent-tick gap.
// Do NOT attempt to align mantissas — there is no mantissa.
// Early-out threshold for lossless add/sub is ~log2(1 + 1/MaxDenom) ≈ 1 tick.
// For MaxDenom = 2^25, Δ >= 26 ticks means the smaller term cannot change the result.

  // Pure magnitude adder, assumes caller resolved sign(and zero check).
  // Default lhsIsPositive=true makes unsigned mode trivial.
  inline void AddMagnitude(const RestrictedFloat& rhs, bool lhsIsPositive=true)
  requires(StandaloneMode)
  {
    auto lExp = this-> expMag();
    auto lFrac = this->expFrac();
    auto rExp = rhs.expMag();
    auto rFrac = rhs.expFrac();
    bool lhsInIntegerLane = this-> expMag();
  }

  // Pure magnitude subtracter, assumes caller resolved sign(and zero check).
  // Default lhsIsPositive=true makes unsigned mode trivial.
  inline void SubtractMagnitude(const RestrictedFloat& rhs, bool lhsIsPositive=true)
  requires(StandaloneMode)
  {
    auto lExp = this-> expMag();
    auto lFrac = this->expFrac();
    auto rExp = rhs.expMag();
    auto rFrac = rhs.expFrac();
  }

  inline void AddInPlace(const RestrictedFloat& rhs)
  requires(StandaloneMode)
  {
    if(rhs.isZero()) return;
    if (isZero()) {
      // 0 + rhs → just rhs magnitude
      *this = rhs; return;
    } else {
      const bool sameSign = SignedMode?IsPositive()==rhs.IsPositive():true;
    }
    if constexpr(SignedMode){
      bool PositiveLeftVal = IsPositive();
      bool PositiveRightVal = rhs.IsPositive();
      if(PositiveLeftVal==PositiveRightVal)//X + (Y)
        AddMagnitude(rhs, PositiveLeftVal);
      else {
        SubtractMagnitude(rhs, PositiveLeftVal)
      }
    } else {
      AddMagnitude(rhs);
    }
  }

  inline void SubInPlace(const RestrictedFloat& rhs)
  requires(StandaloneMode)
  {
    if(rhs.isZero()) return;
    if (isZero()) {
      if constexpr(SignedMode){
        //StandaloneMode allows one existance(this is check to see if rhs is one
        if(rhs.IsNegativeOne())
          SetAsOne();
        else if(rhs.IsOne())
          SetAsNegativeOne();
        } else {
          bool PositiveRightVal = rhs.IsPositive();
          if(PositiveRightVal){
            // Inverting fractional lane across integer boundary: 0 - rhs
            if constexpr(SignedExpMode){
            else if constexpr(SplitRawMode){
            } else {//-1 to 1 range
              StoreT uRHS = rhs.expMag() * MaxDenom + rhs.expFrac();
              StoreT u     = u_minus_mag_from_gap_ticks(uRHS);
              unsigned E     = u / MaxDenom;
              unsigned S     = u % MaxDenom;
              SetStandaloneMagnitudes(E, S, false, false);
            }
          } else
            *this = rhs;
        }
      } else//0 - (Any non-zero) = negative number (which requires Signed Mode) 
        throw("RestrictedFloat Negative Overflow Exception");
      return;
    }
    if constexpr(SignedMode){
      bool PositiveLeftVal = IsPositive();
      bool PositiveRightVal = rhs.IsPositive();
      if(PositiveLeftVal!=PositiveRightVal)//X - (-Y)
        AddMagnitude(rhs, PositiveLeftVal);
      else {
        SubtractMagnitude(rhs, PositiveLeftVal)
      }
    } else {
      SubtractMagnitude(rhs);
    }
  }

  // Returns: 0 = no coarse change, 1 = promote +1 coarse unit, 2 = borrow -1 coarse unit
  template<typename ValueT, typename TickT=StoreT>
  inline uint8_t TailAddSubInPlace(const RestrictedFloat& rhs,
  bool leftIsPositive=true,bool rightIsPositive=true)
  requires(!StandaloneMode)
  {
    if(rhs.isZero()) return 0;
    const bool sameSign = leftIsPositive==rightIsPositive;
    if (isZero()) {
      // 0 + rhs → just rhs magnitude
      if(sameSign) {*this = rhs; return 0; }
      // Inverting fractional lane across integer boundary: 0 - rhs
      uint32_t uRHS = rhs.expMag() * MaxDenom + rhs.expFrac();
      uint32_t u     = u_minus_mag_from_gap_ticks(uRHS);
      unsigned E     = u / MaxDenom;
      unsigned S     = u % MaxDenom;
      SetMagnitudes(E, S);
      return 2;
    }
    if (rhs.isZero()) { return 0; }
    auto lExp = this-> expMag();
    auto lFrac = this->expFrac();
    auto rExp = rhs.expMag();
    auto rFrac = rhs.expFrac();
    
    if (lFrac != 0 && rFrac != 0) {
      // Both operands in fractional-exponent band: at least one signif > 0
      if(lExp==rExp){
        //Both sides in same band
        if(lFrac!=rFrac){
          //ToDo:Add code
        }
        //Both sides have same value so either double or negate
        else if(leftIsPositive!=rightIsPositive)//Canceling tail
          SetAsZero();
          return 0;
        } else {//Doubling
          //ToDo:Add code
        }
      } else if(lExp<rExp) {
        //Left side is bigger than right
        //ToDo:Add code
      } else {
        //Right side is bigger than left
        //ToDo:Add code
      }
    } else {
      // Align smaller-exp side to larger-exp side
      //Shift numerator to same denom such as 1/2 + 1/4 becomes 2/4 + 1/4
      //Shift numerator to same denom such as 1/2 + 1/8 becomes 4/8 + 1/8
      //Shift numerator to same denom such as 1/2 - 1/4 becomes 2/4 - 1/4
      //Shift numerator to same denom such as 1/8 - 1/2 becomes 1/8 - 4/8 = -3/8
      if (lExp < rExp) {
        auto shift = rExp - lExp;
        StoreT numResult = sameSign ? (1 << shift) + 1: (1 << shift) - 1;
        ConvertPureExpPair(numResult, rExp);
        if(sameSign||rightIsPositive) return 0;//1/2 + 1/4 becomes 2/4 - 1/4 = 1/4 ; 1/2 - 1/4 becomes 2/4 - 1/4 = 1/4
        else
          return 2;
      } else if (rExp < lExp) {
        auto shift = lExp - rExp;
        StoreT numResult = sameSign ? (1 << shift) - 1: (1 << shift) + 1;
        ConvertPureExpPair(numResult, lExp);
        if(sameSign) return 0;
        else if(rightIsPositive)//-1/8 + 1/2 becomes -1/8 + 4/8 = 3/8
          return 1;
        else
          return 2;
      } else if(leftIsPositive!=rightIsPositive){//canceling tail
          SetAsZero();//0.5 - 0.5 
          return 0;
      } else {//Doubling
          //(exp:1) 0.5 + 0.5 = (exp:0) 1.0
          //(exp:2) 0.25 + 0.25 = (exp:-1) 0.5
          //(exp:3) 0.0.125 + 0.0.125 = (exp:-1) 0.25
          if(lExp==1){
            SetAsZero();
            return 1;
          }
          SetExp(lExp-1);
          return 0;
      }
    }
  }
  
  inline OverflowResult AddOp(const IntType& rhs, bool leftIsPositive=true, bool rightIsPositive=true){
	  TailAddSubInPlace(rhs, leftIsPositive, rightIsPositive);
  }

  inline OverflowResult SubOp(const RestrictedFloat &rhs, bool leftIsPositive=true, bool rightIsPositive=true){
	  TailAddSubInPlace(rhs, leftIsPositive, !rightIsPositive);
  }

  #pragma endregion MixedDec Operator Helpers

};

  #pragma region ValueDefine Source

  RestrictedFloat RestrictedFloat::Zero = ZeroValue();

  #pragma endregion ValueDefine Source

} // namespace BlazesRusCode

//ExponentalDec
//96 Bits total size
//16 bits of ExtendedRange
//9 Bits of ExpMag
//55 Bits of ExpFrac
//2 Bits towards signs
//1 Bit towards ZeroSentinal
//+ ExtraFlags