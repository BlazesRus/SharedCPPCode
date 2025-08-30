// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

   /**
   * MediumDecV3Variant: High-precision decimal type with optional extended representation.
   *
   * Configuration flags:
   *   – UnsignedMode
   *       • false: two’s-complement–style bitwise encoding for negatives
   *         (negative values sort below positives; high bit = sign).
   *       • true: sign bit is unused and number is always treated as unsigned in storage
   *   – DisableBitwiseMaskMode
   *       • false: all sub-fields are masked out of a single raw_ bitstream.
   *       • true: IntHalf/DecimalHalf occupy their full integer widths.
   *   – EnableExtraRep
   *       • true: adds ExtraRep for alternate scaling or metadata.
   *   – EnableMixedMode
   *       • true: enables output of lost digits
   *   - Check the Policy.hpp file for reference for other Policy flag information
   *
   *
   * Storage layout (bitwise-mode unless DisableBitwiseMaskMode):
   *
   *   raw (uint64_t by default; uint96_t in ExpandedUDec setup)
   *     • underlying bit container for all masked sub-fields.
   *
   *   Sign:
   *     • when bitwise-mode: Bitvalue mask for sign bit
   *     (If PositiveSign Policy is in effect, than if this is one than the number is treated as positive which is opposite
   *      of standard integer bit flags) 
   *     • when mask-disabled : alias for IntHalf.Sign (IntHalf.IsPositive())
   *
   *   Value (mask, unsigned 62 bits by default)
   *     • combination of IntValue + DecimalHalf masks.
   *     • can be treated as an unsigned numeric integer; used for bitwise tests, comparisons, shifts, and simplifying operations.
   *
   *   IntHalf (uint32_t default if mask disabled)
   *     • when bitwise-mode: mask of the full integer sub-field in raw_.
   *     • when mask-disabled : full integer portion, scaled by 10^0.
   *
   *   IntValue (mask, unsigned 31 bits by default)
   *     • when bitwise-mode: alias for unsigned part of IntHalf
   *     • when mask-disabled : alias for IntHalf.Value
   *
   *   DecimalHalf (uint32_t mask by default)
   *     • when bitwise-mode: mask of the fractional sub-field in raw_.
   *     • when mask-disabled : fractional numerator (scaled by 10^-9).
   *     • value > 999'999'999 ? alternate scale or special form sentinel.
   *
   *   ExtraRep (uint32_t, optional)
   *     • divisor or metadata flags when EnableExtraRep is true.
   *     • == 0 ? canonical zero sentinel (IntHalf = 0, DecimalHalf = 0).
   *     • nonzero ? alternate scaling, extended fraction, or special modes.
   *
   * Core features:
   *   – integer-only arithmetic with sub-ppm precision
   *   – branchless algorithms, LUT/polynomial approximations
   *   – exact representation of canonical breakpoints (0.1, 0.5, 1.0)
   *   – deterministic across platforms (no floating-point)
   *   – optional extended fractional/metadata via ExtraRep
   *
   * Template fields:
   *   - VariantClass: Name of the final class
   *    (used for self references to allow to derive into other files while auto updating references until final)
   *
   * Static member:
   *   static constexpr MediumDecV3 Zero;  // { IntHalf = 0, DecimalHalf = 0, ExtraRep = 1 }
   *
   * Typical use cases:
   *   – hybrid numeric engines
   *   – symbolic math libraries
   *   – error-tracked computation pipelines
   *   – Non-float converters for LUT/RestrictedFloat methods
  */