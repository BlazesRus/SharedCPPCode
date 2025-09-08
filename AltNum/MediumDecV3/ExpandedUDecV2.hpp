// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV3Variant.hpp"

namespace BlazesRusCode
{
  #if !defined(AltNum_UseRepTypeAsClass)
  using RepTypeEnum = BlazesRusCode::RepType;
  #endif

  using MirroredInt = BlazesRusCode::MirroredInt;
  using PartialInt = BlazesRusCode::PartialInt;
  using FlaggedInt = BlazesRusCode::FlaggedInt;

  #if defined(__SIZEOF_INT128__)
    using u128 = unsigned __int128;   // native, fastest when available
    #define HAVE_NATIVE_UInt128 1
  #else
    using u128 = boost::multiprecision::uint128_t;
    #define HAVE_NATIVE_UInt128 0
  #endif
  using u32  = std::uint32_t;
  using u64  = std::uint64_t;

  // ExpandedUDec: High-precision unsigned decimal type with extended representation.
  // - IntHalf: Integer portion (64-bit), scaled by 10^0.
  // - DecimalHalf: Fractional portion (32-bit), scaled by 10^-9. if DecimalHalf >= 0
  // ExtraRep:
  // - Metadata flags controlling fractional divisor and special encodings.
  // - ExtraRep = 0 => canonical zero sentinel (IntHalf = 0, DecimalHalf = 0,).
  // - Non-zero ExtraRep values may indicate alternate scaling, special forms, etc.
  // 
  // This type supports:
  // - Integer-only arithmetic with sub-ppm precision.
  // - Branchless operations and LUT/polynomial approximations.
  // - Exact representation of canonical breakpoints (e.g. 0.1, 0.5, 1.0).
  // - Deterministic behavior across platforms (no float usage).
  //
  // Static member:
  // - Zero: Represents the canonical zero value (IntHalf = 0, DecimalHalf = 0, ExtraRep = 0).
  //
  // Used in: Hybrid numeric engines, symbolic math, and error-tracked computation pipelines.
  struct ExpandedUDecPolicy : FixedDecDefaultPolicy{
    // Core
    static constexpr bool UnsignedMode            = true;
    using StoreT                                   = u64;          // unused in split mode but required
    static constexpr bool SplitFieldsMode  = true;

    // Split layout types
    using IntHalfT     = u64;                                     // IntHalf (matches ExpandedUDec)
    using DecimalHalfT = u32;                                     // DecimalHalf
    using ExtraRepT    = u32;                                     // ExtraRep divisor

    // ExtraRep as divisor on by default
    static constexpr bool EnableExtraRep          = true;
    static constexpr bool EnablePowerOf           = false;
    static constexpr bool EnableMixedFrac         = false;

    // Special/state features (off in ExpandedUDec)
    static constexpr bool EnablePi                = false;
    static constexpr bool EnableE                 = false;
    static constexpr bool EnableI                 = false;
    static constexpr bool EnableInfinity          = false;
    static constexpr bool EnableApproaching       = false;
    static constexpr bool EnableApproachingDivided= false;
    static constexpr bool EnableNaN               = false;
    static constexpr bool EnableWithinMinMaxRange = false;

    // Mixed mode (not needed here)
    static constexpr bool EnableMixedMode         = false;

    // Bit sizes (not used in split layout but required by concept)
    static constexpr unsigned IntHalfBits         = 64;
    static constexpr unsigned DecimalHalfBits     = 32;
    static constexpr unsigned ValueBits           = 96;// bookkeeping only
    static constexpr unsigned stateFlagBits       = 0;

    // Numeric limits
    static constexpr unsigned DecimalOverflow     = 1'000'000'000;
    static constexpr u64      MaxIntHalf       = 18'446'744'073'709'551'615ULL;
    static constexpr unsigned FractionalMaximum   = 2'147'483'647;
  };

  struct ExpandedUDecV2Policy : FixedDecDefaultPolicy {
      // Core identity
      static constexpr bool UnsignedMode           = true;
      static constexpr bool SplitFieldsMode = false; // keep packed path
      static constexpr bool EnableMaskedExtraRep   = true;

      using StoreT                    = unsigned __int128; // or custom wide-int
  
      // Field widths for “expanded bitspace”
      static constexpr unsigned IntHalfBits        = 64;
      static constexpr unsigned DecimalHalfBits    = 32;
      static constexpr unsigned ValueBits          = 96;
      static constexpr unsigned stateFlagBits      = 0; // no specials by default
      static constexpr unsigned MaskedExtraBits = 34;//Stealing space from state to get even bigger divisor
  
      // Extra divisor / scaling field
      static constexpr bool EnableExtraRep         = true;
      using ExtraRepT                               = u32;
  
      // Special features
      static constexpr bool EnableInfinity         = false;
      static constexpr bool EnableApproaching      = false;
      static constexpr bool EnableNaN              = false;
  
      // Limits
      static constexpr unsigned DecimalOverflow    = 1'000'000'000;
      static constexpr u64      MaxIntHalf         = 18'446'744'073'709'551'615ULL;
      static constexpr unsigned FractionalMaximum  = 17'179'869'184;
  };
}
