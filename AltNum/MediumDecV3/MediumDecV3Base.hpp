// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <cstdint>
#include <limits>
#include <bit>  // C++20 std::bit_width
#include <climits> // for CHAR_BIT
#include <string>
#include <type_traits> //for std::enable_if_t etc
#if !defined(__SIZEOF_INT128__)
  #include <boost/multiprecision/cpp_int.hpp>
#endif
#include <concepts> //C++20 concept

#include "..\AlternativeInt\MirroredInt.hpp"
#include "..\AlternativeInt\PartialInt.hpp"
#include "..\AlternativeInt\FlaggedInt.hpp"

#include "..\RepType.h"
#include "..\..\OtherFunctions\VariableConversionFunctions.hpp"

#include "CheckExistance.hpp"
#include "DefaultPolicy.hpp"
#include "ExtraRepFields.hpp"

//Documentation.hpp now holds extended MediumDecV3Variant's extended summary comment for now

namespace BlazesRusCode {
namespace AltNum {
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
  using int32  = std::int32_t;
  using int64  = std::int64_t;

  //CheckMemberOrMethodExistance is now CheckExistance inside CheckExistance.hpp

  //FixedDecDefaultPolicy is now in DefaultPolicy.hpp
  //DefaultDecimalPowerTable is now in DefaultDecimalPowerTable.hpp
  //FixedDecPolicyVariant is now in DefaultPolicy.hpp

  //ExtraRepFields and NoExtraRep are now in ExtraRepFields.hpp

  struct StorageConcepts
  {
  template<typename P, typename = void>
  struct has_overflow_limit : std::false_type {};
  
  template<typename P>
  struct has_overflow_limit<P, std::void_t<decltype(P::DecimalOverflow)>>
  : std::true_type {};

  template<class T>
  struct has_value_member<T, std::void_t<decltype(std::declval<T&>().Value)>> : std::true_type {};
  
  template<class T>
  using value_member_t = std::remove_reference_t<decltype(std::declval<T&>().Value)>;
  
  // Can we brace-construct T from U?
  template<class T, class U, class = void>
  struct brace_constructible_from : std::false_type {};
  
  template<class T, class U>
  struct brace_constructible_from<T, U, std::void_t<decltype(T{std::declval<U>()})>> : std::true_type {};
  }

  //Core storage: either a single raw blob or split fields
  template<typename Policy>
  struct MDBaseStorage
  {
    // Concepts for presence detection (C++20)
    template<typename Policy>
    concept Has_PositiveSign =
        requires {
            { Policy::PositiveSign } -> std::integral;
        } &&
        ((Policy::PositiveSign == 0) || (Policy::PositiveSign == 1));
    
    template<typename Policy>
    concept Has_NegativeSign =
        requires {
            { Policy::NegativeSign } -> std::integral;
        } &&
        ((Policy::NegativeSign == 0) || (Policy::NegativeSign == 1));
    
    concept Has_EnablePi   = requires { { Policy::EnablePi }   -> std::convertible_to<bool>; };
    concept Has_EnableE    = requires { { Policy::EnableE }    -> std::convertible_to<bool>; };
    concept Has_EnablePiE  = requires { { Policy::EnablePiE }  -> std::convertible_to<bool>; };
    concept Has_EnableI    = requires { { Policy::EnableI }    -> std::convertible_to<bool>; };
    
    concept Has_EnableInfinity        = requires { { Policy::EnableInfinity }        -> std::convertible_to<bool>; };
    concept Has_EnableApproaching     = requires { { Policy::EnableApproaching }     -> std::convertible_to<bool>; };
    concept Has_DisableApproachingTop = requires { { Policy::DisableApproachingTop } -> std::convertible_to<bool>; };
    concept Has_EnableNaN             = requires { { Policy::EnableNaN }             -> std::convertible_to<bool>; };
    concept Has_EnableApproachingDivided = requires { { Policy::EnableApproachingDivided } -> std::convertible_to<bool>; };
    concept Has_EnableIndeterminateForms = requires { { Policy::EnableIndeterminateForms } -> std::convertible_to<bool>; };
    
    concept Has_UnsignedMode = requires { { Policy::UnsignedMode } -> std::convertible_to<bool>; };
    concept Has_SignedMode   = requires { { Policy::SignedMode   } -> std::convertible_to<bool>; };
    
    concept Has_SplitFieldsMode = requires { { Policy::SplitFieldsMode } -> std::convertible_to<bool>; };
    concept Has_BitwiseMaskMode = requires { { Policy::BitwiseMaskMode } -> std::convertible_to<bool>; };
    
    concept Has_EnableExtraRep         = requires { { Policy::EnableExtraRep }         -> std::convertible_to<bool>; };
    concept Has_EnableMaskedExtraRep   = requires { { Policy::EnableMaskedExtraRep }   -> std::convertible_to<bool>; };
    concept Has_EnableMixedFrac        = requires { { Policy::EnableMixedFrac }        -> std::convertible_to<bool>; };
    concept Has_EnablePowerOf          = requires { { Policy::EnablePowerOf }          -> std::convertible_to<bool>; };
    concept Has_EnableWithinMinMaxRange    = requires { { Policy::EnableWithinMinMaxRange } -> std::convertible_to<bool>; };
    concept Has_EnableNotWithinMinMaxRange = requires { { Policy::EnableNotWithinMinMaxRange } -> std::convertible_to<bool>; };
    
    concept Has_MaxIntHalfMag   = requires { { Policy::MaxIntHalfMag }   -> std::convertible_to<unsigned>; };
    concept Has_IntValue_BITS   = requires { { Policy::IntValue_BITS }   -> std::convertible_to<unsigned>; };
    concept Has_DecimalHalf_BITS= requires { { Policy::DecimalHalf_BITS }-> std::convertible_to<unsigned>; };
    concept Has_FlagState_BITS  = requires { { Policy::FlagState_BITS }  -> std::convertible_to<unsigned>; };
    concept Has_MaskedExtra_BITS= requires { { Policy::MaskedExtra_BITS }-> std::convertible_to<unsigned>; };
    concept Has_ExtraFlag_BITS  = requires { { Policy::ExtraFlag_BITS }  -> std::convertible_to<unsigned>; };
    concept Has_MixedFracDivisorLimit = requires { { Policy::MixedFracDivisorLimit } -> std::convertible_to<unsigned>; };
    
    concept Has_BinaryFractionMode      = requires { { Policy::BinaryFractionMode }      -> std::convertible_to<bool>; };
    concept Has_EnableOverflowProtection= requires { { Policy::EnableOverflowProtection }-> std::convertible_to<bool>; };

    concept Has_SkipSafeCheck = requires { { Policy::Has_SkipSafeCheck }-> std::convertible_to<bool>; };

    //Both bitwise and RawBlobe share features here
		//Generate global policy based static const here
    struct GlobalStorage {
    protected:
      // PositiveSign and NegativeSign are logical convention constants (1/0 or true/false).
      // They define whether a sign bit value of 1 means "positive" (this is inverted from the usual two's-complement convention).
      // The "sign bit" here refers to the logical sign indicator for this type, not necessarily a literal bit in all modes.
      // Defaults: PositiveSign = 1, NegativeSign = 0 if neither is provided in the policy.
      // Exactly one may be defined in the policy; the other is derived to be its opposite.
      // In signed mode, negative values do NOT have their magnitude bits flipped — only the sign indicator changes.
      // This avoids magnitude inversion overhead and enhances multiplication/division speed for negative values.
      static inline constexpr u32 PositiveSign   = Has_PositiveSign ? Policy::PositiveSign : (Has_NegativeSign?0:1);
      static inline constexpr u32 NegativeSign   = Has_PositiveSign ? 0 : (Has_NegativeSign?1:0);

      if constexpr (Has_PositiveSign)
          static_assert(Policy::PositiveSign == 0 || Policy::PositiveSign == 1,
                        "PositiveSign must be 0 or 1");
      if constexpr (Has_NegativeSign)
          static_assert(Policy::NegativeSign == 0 || Policy::NegativeSign == 1,
                        "NegativeSign must be 0 or 1");

      static inline constexpr bool EnablePi = Has_EnablePi ? Policy::EnablePi  : false;
      static inline constexpr bool EnableE = Has_EnableE ? Policy::EnableE  : false;
      static inline constexpr bool EnablePiE = Has_EnablePiE&&EnablePi&&EnableE ? Policy::EnablePiE  : false;
      static inline constexpr bool EnableI = Has_EnableI&&EnablePiE==false ? Policy::EnableI  : false;
      
      static inline constexpr bool EnableInfinity = Has_EnableInfinity ? Policy::EnableInfinity  : false;
      static inline constexpr bool EnableApproaching = Has_EnableApproaching ? Policy::EnableApproaching  : false;
      static inline constexpr bool DisableApproachingTop = 
      Has_DisableApproachingTop ? Policy::DisableApproachingTop  : false;

      static inline constexpr bool EnableNaN = Has_EnableNaN ? Policy::EnableNaN  : false;

      static inline constexpr bool EnableApproachingDivided = 
      Has_EnableApproachingDivided ? Policy::EnableApproachingDivided  : false;

      static inline constexpr bool EnableIndeterminateForms = 
      Has_EnableIndeterminateForms ? Policy::EnableIndeterminateForms  : false; 

      //If enabled OpMethods return Trailing digits lost during operation for use in MixedDec variant operations
      static constexpr bool EnableMixedMode = false;

      static inline constexpr bool EnableStateFlags   =
      EnablePi || EnableE || EnableI;

			using IntHalfT = std::conditional_t<
					requires { typename Policy::IntHalfT; },
					typename Policy::IntHalfT,
					u32>;

			using DecimalHalfT = std::conditional_t<requires { typename Policy::DecimalHalfT; },
					typename Policy::DecimalHalfT,u32>;

			using IntHalfValueT = std::conditional_t<requires { typename Policy::IntHalfValueT; },
					typename Policy::IntHalfValueT, typename IntHalfT>;

			using DecimalHalfValueT = std::conditional_t<requires { typename Policy::DecimalHalfValueT; },
					typename Policy::DecimalHalfValueT, typename IntHalfT>;

			using IntHalfSignedT = std::conditional_t<requires { typename Policy::IntHalfValueT; },
					typename Policy::IntHalfValueT,int32>;

			template<class T>
			concept HasValueMember =
					requires(T t) {
							{ t.Value } -> std::convertible_to<IntHalfValueT>;
					};
					
			template<class T>
			concept HasDecimalValueMember =
					requires(T t) {
							{ t.Value } -> std::convertible_to<DecimalHalfValueT>;
					};

      // Opposite Modes to make constexpr checks easier.
      // SignedMode is always the logical inverse of UnsignedMode.
      // BitwiseMaskMode is always the logical inverse of SplitFieldsMode.
      // Exactly one of each pair may be defined in the policy; the other is derived.
      // Defaults: UnsignedMode = false (SignedMode = true), 
      // SplitFieldsMode = false (BitwiseMaskMode = true) if neither is defined.
      static_assert(!(Has_UnsignedMode && Has_SignedMode),
      "Policy cannot define both UnsignedMode and SignedMode");
			
      static inline constexpr bool UnsignedMode =
      Has_UnsignedMode ? !!Policy::UnsignedMode : (Has_SignedMode ? !Policy::SignedMode : false);

      static inline constexpr bool SignedMode = !UnsignedMode;

      static_assert(!(Has_SplitFieldsMode && Has_BitwiseMaskMode),
      "Policy cannot define both SplitFieldsMode and BitwiseMaskMode");

      static inline constexpr bool SplitFieldsMode =
      Has_SplitFieldsMode ? !!Policy::SplitFieldsMode : (Has_BitwiseMaskMode ? !Policy::BitwiseMaskMode : false);

      static inline constexpr bool BitwiseMaskMode = !SplitFieldsMode;

      //If true skip safe zero check
      static inline constexpr bool SkipSafeCheck = Has_SkipSafeCheck ? Policy::SkipSafeCheck : true;

      static inline constexpr bool EnableExtraRep = Has_EnableExtraRep ? Policy::EnableExtraRep||Has_EnableMaskedExtraRep : false;
		  using ExtraRepT = Policy::EnableExtraRep ? (Policy::ExtraRepT ? typename ExtraRepT: typename u32) : void;
			
      static inline constexpr bool EnableMixedFrac     = Has_EnableMixedFrac&&EnableExtraRep ? Policy::EnableMixedFrac : false;
      static inline constexpr bool EnablePowerOf     = Has_EnablePowerOf&&EnableExtraRep ? Policy::EnablePowerOf : false;
		  static inline constexpr bool EnableExtraRepFlags = EnablePowerOf || EnableMixedFrac;
			
      static inline constexpr unsigned FractionalMaximum = Has_FractionalMaximum ? Policy::FractionalMaximum : 
      (EnableExtraRep?2147483647:0);

      //Defined when ExtraRep is equal to 0, Represents a number 
      //within bounds of (Sign*IntHalf.Value) to (ExtraRep.IsAltRep?-1:1)*(DecimalHalf.Value)
      static inline constexpr bool EnableWithinMinMaxRange = 
      Has_EnableWithinMinMaxRange&&EnableExtraRep ? Policy::EnableWithinMinMaxRange  : false;

      //Defined when ExtraRep is equal to ??????????, Represents a number 
      //not within bounds of (Sign*IntHalf.Value) to (ExtraRep.IsAltRep?-1:1)*(DecimalHalf.Value)
      static inline constexpr bool EnableNotWithinMinMaxRange = 
      Has_EnableNotWithinMinMaxRange&&EnableExtraRep ? Policy::EnableNotWithinMinMaxRange  : false;

      //Max IntHalf magnitude
      static constexpr unsigned MaxIntHalfMag = Has_MaxIntHalfMag? Policy::MaxIntHalfMag:
      (UnsignedMode? 4294967295:2147483647); 

      //ExtraRep only exists as variable if EnableExtraRep is enabled
      using ExtraRepT = std::conditional_t<requires { typename Policy::ExtraRepT; }, typename Policy::ExtraRepT, u32>;

      //Unsigned value type that is big enough to hold both IntHalf*DecimalOverflow+DecimalHalf 
      using ValueT = std::conditional_t<requires { typename Policy::ValueT; }, typename Policy::ValueT, u64>;
    
      //Experimental Binary fractional mode(DecimalHalf as powers of 2 instead of 10)
      static inline constexpr bool BinaryFractionMode =
      Has_BinaryFractionMode ? Policy::BinaryFractionMode : false;

      static constexpr bool EnableOverflowProtection =
      Has_EnableOverflowProtection ? Policy::EnableOverflowProtection : true;

    };
    // when bitwise mode:
    struct RawBlob : public GlobalStorage {
    //IntValue is magnitude of Integer Half even when negative
    protected:
			using StoreT = std::conditional_t<requires { typename Policy::StoreT; },
					typename Policy::StoreT,u64>;

      //Checking if DecimalHalf is supported type before using table
      static constexpr DecimalHalfT DecimalOverflow = []{
        if constexpr (has_overflow_limit<Policy>::value) {
          return static_cast<DecimalHalfT>(Policy::DecimalOverflow);
        } else if constexpr (std::is_integral_v<DecimalHalfT>) {
          return static_cast<DecimalHalfT>(
          VariableConversionFunctions::PowerOfTens64Bit[Policy::DecimalHalfDigits + 1]);
        } else {
          return static_cast<DecimalHalfT>(
          template DefaultDecimalPowerTable<Policy::DecimalHalfT, Policy::DecimalHalfDigits>
          {}.values[Policy::DecimalHalfDigits + 1]);
        }
      }();

      static constexpr DecimalHalfT DecimalMax = DecimalOverflow - 1;

      static constexpr unsigned kBlobBits = sizeof(StoreT) * CHAR_BIT;

      static constexpr StoreT ones(unsigned bits) {
        return bits == 0 ? StoreT(0)
           : bits >= kBlobBits ? ~StoreT(0)
           : (StoreT(1) << bits) - StoreT(1);
      }

      //Checking if DecimalHalf is supported type before using table
      static constexpr DecimalHalfT DecimalOverflow = []{
        if constexpr (has_overflow_limit<Policy>::value) {
          return static_cast<DecimalHalfT>(Policy::DecimalOverflow);
        } else if constexpr (std::is_integral_v<DecimalHalfT>) {
          return static_cast<DecimalHalfT>(
          VariableConversionFunctions::PowerOfTens64Bit[Policy::DecimalHalfDigits + 1]);
        } else {
          return static_cast<DecimalHalfT>(
          template DefaultDecimalPowerTable<Policy::DecimalHalfT, Policy::DecimalHalfDigits>
          {}.values[Policy::DecimalHalfDigits + 1]);
        }
      }();

      //Bitlayout(from low bits to high bits):
      //Sign(SIGN_BITS Bits)->ScaleValue(IntHalf(INT_BITS Bits)+DecimalHalf(DECIMAL_BITS Bits))
      //->Flag State(FLAG_BITS Bits)->Masked ExtraRep(EXTRA_BITS Bits)->AltRepFlag(s)(EXTRAFLAG_BITS Bits)

      // --- Effective flags ---
      static inline constexpr bool EnableMaskedExtraRep =
        Has_EnableMaskedExtraRep ? Policy::EnableMaskedExtraRep&&EXTRA_BITS > 0 : false;

      static constexpr unsigned MixedFracDivisorLimit = Has_MixedFracDivisorLimit ? Policy::MixedFracDivisorLimit :
          (EnableMixedFrac ? (EnableMaskedExtraRep ? ((1u << EXTRA_BITS) - 1u) : 1073741804u) : 0u);

      // --- Widths ---
      static inline constexpr unsigned SIGN_BITS  = UnsignedMode ? 0u : 1u;
      static inline constexpr unsigned INT_BITS   = Has_IntValue_BITS ? Policy::IntValue_BITS : (UnsignedMode?32:31);
      static inline constexpr unsigned DECIMAL_BITS = Has_DecimalHalf_BITS? Policy::DecimalHalf_BITS : 30;
      static inline constexpr unsigned VALUE_BITS   = INT_BITS + DECIMAL_BITS;
      static inline constexpr unsigned FLAG_BITS  = EnableStateFlags ? (Has_FlagState_BITS?Policy::FlagState_BITS:2) : 0u;
      static inline constexpr unsigned EXTRA_BITS   = EnableMaskedExtraRep&&Has_MaskedExtra_BITS ? Policy::MaskedExtra_BITS: 0u;
      static inline constexpr unsigned EXTRAFLAG_BITS   = EnableMaskedExtraRep&&Has_ExtraFlag_BITS ? Policy::ExtraFlag_BITS : 0u;
    
      // --- Shifts (Sign on top) ---
      static inline constexpr unsigned SHIFT_SIGN  = INT_BITS + DECIMAL_BITS + FLAG_BITS + EXTRA_BITS + EXTRAFLAG_BITS;
      static inline constexpr unsigned SHIFT_INT   = DECIMAL_BITS + FLAG_BITS + EXTRA_BITS + EXTRAFLAG_BITS;
      static inline constexpr unsigned SHIFT_DECIMAL = FLAG_BITS + EXTRA_BITS + EXTRAFLAG_BITS;
      static inline constexpr unsigned SHIFT_VALUE  = SHIFT_DECIMAL; // low end starts at DecimalHalf
      static inline constexpr unsigned SHIFT_FLAG  = EXTRA_BITS + EXTRAFLAG_BITS;
      static inline constexpr unsigned SHIFT_EXTRA   = EXTRAFLAG_BITS;
      static inline constexpr unsigned SHIFT_EXTRAFLAG   = 0u;

      //Static storage-width check
      static_assert(
      (SIGN_BITS + INT_BITS + DecimalHalf_BITS + FlagState_BITS + EXTRA_BITS + EXTRAFLAG_BITS) <= (sizeof(StoreT)*CHAR_BIT),
      "Policy::StoreT too narrow for configured bit-fields"
      );

      // --- PURE masks ---
      static inline constexpr StoreT PURE_SIGN_MASK  = ones(SIGN_BITS);
      static inline constexpr StoreT PURE_INT_MASK   = ones(INT_BITS);
      static inline constexpr StoreT PURE_DEC_MASK   = ones(DECIMAL_BITS);
      static inline constexpr StoreT PURE_VALUE_MASK = ones(VALUE_BITS);
      static inline constexpr StoreT PURE_FLAG_MASK  = ones(FLAG_BITS);
      static inline constexpr StoreT PURE_EXTRA_MASK = ones(EXTRA_BITS);
      static inline constexpr StoreT PURE_EXTRAFLAG_MASK = ones(PURE_EXTRAFLAG_MASK);

      // --- Shifted masks ---
      static inline constexpr StoreT Sign_MASK  = PURE_SIGN_MASK  << SHIFT_SIGN;
      static inline constexpr StoreT Int_MASK   = PURE_INT_MASK   << SHIFT_INT;
      static inline constexpr StoreT Dec_MASK   = PURE_DEC_MASK   << SHIFT_DECIMAL;
      static inline constexpr StoreT Value_MASK = PURE_VALUE_MASK << SHIFT_VALUE;
      static inline constexpr StoreT Flag_MASK  = PURE_FLAG_MASK  << SHIFT_FLAG;
      static inline constexpr StoreT Extra_MASK = PURE_EXTRA_MASK << SHIFT_EXTRA;
      static inline constexpr StoreT ExtraFlag_MASK = PURE_EXTRAFLAG_MASK << SHIFT_EXTRA;

      // --- Clamp helpers ---
      static constexpr unsigned clampSign(unsigned v)  noexcept { return unsigned(v & unsigned(PURE_SIGN_MASK)); }
      static constexpr unsigned clampInt(unsigned v)   noexcept { return unsigned(v & unsigned(PURE_INT_MASK)); }
      static constexpr unsigned clampDec(unsigned v)   noexcept { return unsigned(v & unsigned(PURE_DEC_MASK)); }
      static constexpr unsigned clampValue(unsigned v) noexcept { return unsigned(v & unsigned(PURE_VALUE_MASK)); }
      static constexpr unsigned clampFlags(unsigned v) noexcept { return unsigned(v & unsigned(PURE_FLAG_MASK)); }
      static constexpr unsigned clampExtra(unsigned v) noexcept { return unsigned(v & unsigned(PURE_EXTRA_MASK)); }
      static constexpr unsigned clampExtraFlags(unsigned v) noexcept { return unsigned(v & unsigned(PURE_EXTRAFLAG_MASK)); }

      /*Shortcut bitwise math variables*/
      // Packed representation of exactly 1 integer
      static constexpr ValueT OneDecimalPacked = ValueT(1) << DECIMAL_BITS;
      
      // Borrow‑and‑normalize: +1 integer, −DecimalOverflow fractional
      static constexpr ValueT OneIntPackedMinusDecimalOverflow =
          OneDecimalPacked - DecimalOverflow;
      
      // Carry‑and‑normalize: +1 integer, +DecimalOverflow fractional
      static constexpr ValueT OneIntPackedPlusDecimalOverflow =
          OneDecimalPacked + DecimalOverflow;

      // Maximum packed magnitude: max integer half with max fractional half
      static constexpr ValueT MaxPackedValue =
          (ValueT(MaxIntHalf) << DECIMAL_BITS) | (DecimalOverflow - 1);
      
      // Minimum packed magnitude in unsigned mode
      static constexpr ValueT MinPackedValue = 0;

      // 0/1 raw sign bit as stored (ignores PositiveSign semantics)
      constexpr unsigned RawSignBit() const noexcept {
        if constexpr (UnsignedMode) return 0u;
        return ((raw & Sign_MASK) != 0) ? 1u : 0u;
      }
      
      // 0/1 "negative" bit in semantic space (respects PositiveSign)
      constexpr unsigned NegBit() const noexcept {
        if constexpr (UnsignedMode) return 0u;
        return static_cast<unsigned>(IsNegative());
      }

      constexpr void FlipSign() noexcept {
        raw ^= Sign_MASK;                  // toggle sign
      }

      constexpr void absolute() const noexcept {
        if (raw.IsNegative())              // branch only on the rare negative case
          tmp.SwapNegativeStatus();        // flip sign, leave payload alone
      }

      constexpr void absoluteBranchless() const noexcept {
        StoreT negMask = -uint64_t(raw.IsNegative()); // all-ones if negative, 0 otherwise
        raw ^= Sign_MASK & negMask;                  // flips sign only when negMask==all-ones
      }

      // Returns +1 if value is positive, -1 if negative.
      // Honors Policy::PositiveSign semantics:
      //   - PositiveSign == true  => sign bit 1 means positive
      //   - PositiveSign == false => sign bit 0 means positive (classic float style)
      // Unsigned/SIGN_BITS==0 => always +1 (no sign lane)
      constexpr int SignPolarityMultiplier() const noexcept {
        if constexpr ((UnsignedMode)) {
          return 1;
        } else {
          const bool bitSet   = (raw & Sign_MASK) != 0;
          const bool positive = PositiveSign ? bitSet : !bitSet;
          return positive ? 1 : -1;
        }
      }

      // If you want a direct multiplier from the packed word without creating a RawBlob instance:
      static constexpr int SignPolarityFromRaw(StoreT packed) noexcept {
        if constexpr ((UnsignedMode)) {
          return 1;
        } else {
          const bool bitSet   = (packed & Sign_MASK) != 0;
          const bool positive = PositiveSign ? bitSet : !bitSet;
          return positive ? 1 : -1;
        }
      }

      static constexpr int SignPolarityFromBit(unsigned signBit) noexcept {
        if constexpr ((UnsignedMode)) return 1;
        bool bitSet   = (signBit & 1u) != 0;
        bool positive = PositiveSign ? bitSet : !bitSet;
        return positive ? 1 : -1;
      }

      // --- Packers ---
      static constexpr StoreT packSign(unsigned v)  noexcept { return (StoreT(clampSign(v)) << SHIFT_SIGN) & Sign_MASK; }
      static constexpr StoreT packInt(unsigned v)   noexcept { return (StoreT(clampInt(v))  << SHIFT_INT)  & Int_MASK; }
      static constexpr StoreT packDec(unsigned v)   noexcept { return (StoreT(clampDec(v))  << SHIFT_DECIMAL) & Dec_MASK; }
      static constexpr StoreT packValue(unsigned v) noexcept { return (StoreT(clampValue(v))  << SHIFT_VALUE) & Value_MASK; }
      static constexpr StoreT packFlag(unsigned v)  noexcept { return (StoreT(clampFlags(v))<< SHIFT_FLAG) & Flag_MASK; }
      static constexpr StoreT packExtra(unsigned v) noexcept { return (StoreT(clampExtra(v))<< SHIFT_EXTRA) & Extra_MASK; }
      static constexpr StoreT packExtraFlag(unsigned v) noexcept { return (StoreT(clampExtra(v))<< SHIFT_EXTRAFLAG) & ExtraFlag_MASK; }

      static constexpr unsigned splitIntPart(StoreT value) noexcept {
        return unsigned((value >> DECIMAL_BITS) & PURE_INT_MASK);
      }
      static constexpr unsigned splitDecPart(StoreT value) noexcept {
        return unsigned(value & PURE_DEC_MASK);
      }
      
      static constexpr StoreT GenerateWithValue(unsigned intPart, unsigned decPart) noexcept {
        return ((StoreT(intPart) & PURE_INT_MASK) << DECIMAL_BITS) |
           (StoreT(decPart) & PURE_DEC_MASK);
      }

      // Overwrite int+dec halves, preserve sign, flags, extra
      constexpr void SetValue(unsigned intPart, unsigned decPart) noexcept {
          raw &= ~(Int_MASK | Dec_MASK);
          raw |= packInt(intPart) | packDec(decPart);
      }

      // Overwrite sign + int + dec halves in `raw`, preserving flags and ExtraRep
      constexpr void SetSignedValue(unsigned intPart, unsigned decPart = 0, bool isPositive = true) noexcept
      {
          // Clear only the lanes we’re replacing
          StoreT cleared = raw & ~(Sign_MASK | Int_MASK | Dec_MASK);
      
          if constexpr (UnsignedMode || SIGN_BITS == 0u) {
              raw = cleared | packInt(intPart) | packDec(decPart);
          } else {
              const unsigned signLane =
                  isPositive ? (PositiveSign ? 1u : 0u)
                             : (PositiveSign ? 0u : 1u);
      
              raw = cleared | packSign(signLane)
                             | packInt(intPart)
                             | packDec(decPart);
          }
      }

      // Overwrite IntHalf + DecimalHalf lane, preserve sign, flags, extra
      constexpr void SetRawValue(StoreT packedValue) noexcept {
          raw = (raw & ~Value_MASK) | ((packedValue << SHIFT_VALUE) & Value_MASK);
      }
      
      // Overwrite sign + IntHalf + DecimalHalf, preserve flags, extra
      constexpr void SetRawSignedValue(StoreT packedValue, bool isPositive = true) noexcept {
          // Clear sign + value lane
          raw &= ~(Sign_MASK | Value_MASK);
      
          if constexpr (UnsignedMode || SIGN_BITS == 0u) {
              raw |= (packedValue << SHIFT_VALUE) & Value_MASK;
          } else {
              const unsigned signLane =
                  isPositive ? (PositiveSign ? 1u : 0u)
                             : (PositiveSign ? 0u : 1u);
      
              raw |= packSign(signLane)
                   | ((packedValue << SHIFT_VALUE) & Value_MASK);
          }
      }

      static constexpr StoreT assemble(unsigned sign, unsigned i, unsigned d, unsigned f, unsigned e=0) noexcept {
        return packSign(sign) | packInt(i) | packDec(d) | packFlag(f) | packExtra(e) | packExtraFlag(e);
      }
    
      constexpr void set(unsigned sign, unsigned i, unsigned d, unsigned f, unsigned e=0) noexcept {
        raw = assemble(sign, i, d, f, e);
      }

      static constexpr StoreT packFromTwosComplement(StoreT external) noexcept {
        bool neg = (external & Sign_MASK) != 0; // standard convention
        unsigned signLane = PositiveSign ? !neg : neg;
        return (external & ~Sign_MASK) | (StoreT(signLane) << SHIFT_SIGN);
      }

    public:
      StoreT raw{};
      constexpr RawBlob() noexcept = default;
      constexpr explicit RawBlob(StoreT bits) noexcept : raw(bits) {}

      template<typename = std::enable_if_t<!Policy::UnsignedMode>>
      constexpr unsigned Sign() const noexcept {
      return unsigned((raw & FlagState_MASK) >> SHIFT_FLAG);
      }

      constexpr unsigned IntValue() const noexcept {
        return unsigned((raw & Int_MASK) >> SHIFT_INT);
      }

      constexpr unsigned DecimalHalf() const noexcept {
        return unsigned((raw & Dec_MASK) >> SHIFT_DECIMAL);
      }

      //Combined IntHslf+DecimalHalf lanes
      constexpr unsigned RawValue() const noexcept {
        return unsigned((raw & Value_MASK) >> SHIFT_VALUE);
      }

      inline constexpr ValueT ScaledValue()
			{
			  return GetIntHalf()*DecimalOverflow+GetDecimalHalf();
			}

      //Get Integer half of value magnitude
      constexpr IntHalfValueT GetIntHalf() const noexcept
			{
			  return unsigned((raw & Int_MASK) >> SHIFT_INT);
			}

      //Get Decimal half of value magnitude
      constexpr DecimalHalfValueT GetDecimalHalf() const noexcept
			{
			  return unsigned((raw & Dec_MASK) >> SHIFT_DECIMAL);
			}
			
      //Set IntHalf Magnitude
      inline void SetIntHalf(IntHalfValueT val)
			{
			  if constexpr (HasValueMember<decltype(IntHalf)>)
				  IntHalf.Value = val;
				else
				  IntHalf = val;
			}

      //Set DecHalf Magnitude
      inline void SetDecHalf(DecimalHalfValueT val)
			{
			  if constexpr (HasDecimalValueMember<decltype(DecimalHalf)>)
				  DecimalHalf.Value = val;
				else
				  DecimalHalf = val;
			}

      //Returns Pi, e, i, or Normal state value(1,2,3,0)
      constexpr unsigned FlagState() const noexcept {
        return unsigned((raw & Flag_MASK) >> SHIFT_FLAG);
      }

      constexpr unsigned ExtraRep() const noexcept {
        if constexpr (EXTRA_BITS == 0) return 0u;
        return unsigned((raw & Extra_MASK) >> SHIFT_EXTRA);
      }

      // --- Accessors ---
      // PositiveSign means "1 = positive" (inverse of the usual meaning)
      constexpr bool IsPositive() const noexcept {
        if constexpr (UnsignedMode) return true;
        else return (raw & Sign_MASK) == PositiveSign;
      }
      
      constexpr bool IsNegative() const noexcept {
        if constexpr (UnsignedMode) return false;
        else return (raw & Sign_MASK) == NegativeSign;
      }

      //MaxIntHalf and MinIntHalf likely will be rarely used for raw since representing as mask
      //Used for Maximum and Minimum Value fields
      static constexpr unsigned MaxIntHalf = Has_MaxIntHalf ? Policy::MaxIntHalf : 4294967295; 
      static constexpr unsigned MinIntHalf = Has_MaxIntHalf ? Policy::MinIntHalf : 0;

      static constexpr IntHalfT IntHalfZero = Policy::IntHalfT ? Policy::IntHalfT : 0;
      static constexpr DecimalHalfT DecimalHalfZero = Policy::IntHalfT ? Policy::IntHalfT : 0;

      static inline constexpr StoreT ZeroRaw = []() constexpr {
        if constexpr (Base::UnsignedMode) {
          return StoreT(0);
        } else {
          return (StoreT(0) & Value_MASK)
          | (PositiveSign ? Sign_Mask : StoreT(0));
        }
      }();
    };

    // when split mode:
    struct SplitFields : public GlobalStorage {
    public:
      static inline constexpr bool EnableMaskedExtraRep = false;
			
		  static constexpr unsigned MixedFracDivisorLimit =
			(Policy::EnableMixedFrac && Policy::MixedFracDivisorLimit)
				Policy::MixedFracDivisorLimit)
		  : 0;
		
      IntHalfT   IntHalf;
      DecimalHalfT DecimalHalf;

      static_assert(
      SignedMode&&!HasValueMember<IntValueT>,
      "Policy::SignedMode requires Value members for split mode(signed integers not supported)"
      );

      //Get Integer half of value magnitude
      constexpr IntHalfValueT GetIntHalf() const noexcept
			{
			  if constexpr (HasValueMember<decltype(IntHalf)>)
				  return IntHalf.Value;
				else
				  return IntHalf;
			}

      //Get Decimal half of value magnitude
      constexpr DecimalHalfValueT GetDecimalHalf() const noexcept
			{
			  if constexpr (HasDecimalValueMember<decltype(DecimalHalf)>)
				  return DecimalHalf.Value;
				else
				  return DecimalHalf;
			}
			
      inline void SetIntHalf(IntHalfValueT val)
			{
			  if constexpr (HasValueMember<decltype(IntHalf)>)
				  IntHalf.Value = val;
				else
				  IntHalf = val;
			}

      inline void SetDecHalf(DecimalHalfValueT val)
			{
			  if constexpr (HasDecimalValueMember<decltype(DecimalHalf)>)
				  DecimalHalf.Value = val;
				else
				  DecimalHalf = val;
			}

      inline void SetValue(IntHalfValueT intPart, DecimalHalfValueT decPart)
			{
			  SetIntHalf(intPart);
				SetDecHalf(decPart);
			}

      //Checking if DecimalHalf is supported type before using table
      static constexpr DecimalHalfT DecimalOverflow = []{
        if constexpr (has_overflow_limit<Policy>::value) {
          return static_cast<DecimalHalfT>(Policy::DecimalOverflow);
        } else if constexpr (std::is_integral_v<DecimalHalfT>) {
          return static_cast<DecimalHalfT>(
            VariableConversionFunctions::PowerOfTens64Bit[Policy::DecimalHalfDigits + 1]
          );
        } else if constexpr (HasDecimalValueMember<DecimalHalfValueT>::value) {
          if constexpr (std::is_integral_v<DecimalHalfValueT>) {
            return static_cast<DecimalHalfValueT>(VariableConversionFunctions::PowerOfTens64Bit[Policy::DecimalHalfDigits + 1]);
          }
          else {
            return static_cast<DecimalHalfValueT>(
              Policy::template DefaultDecimalPowerTable<
                Policy::DecimalHalfValueT, Policy::DecimalHalfDigits
              >{}.values[Policy::DecimalHalfDigits + 1]
            );
          }
        } else {
          return static_cast<DecimalHalfT>(
            Policy::template DefaultDecimalPowerTable<
              Policy::DecimalHalfT, Policy::DecimalHalfDigits
            >{}.values[Policy::DecimalHalfDigits + 1]
          );
        }
      }();
    
      static constexpr DecimalHalfT DecimalMax = DecimalOverflow - 1;
			
      inline constexpr ValueT ScaledValue()
			{
			  return GetIntHalf()*DecimalOverflow+GetDecimalHalf();
			}

    protected:

      constexpr void EnforceDecimalHalfShape()
      {
        if constexpr (EnableStateFlags) {
          static_assert(
            CheckMemberOrMethodExistance::HasFlagsField<typename Policy::ExtraRepT>,
            "In DecimalHalf must have Flags field to use flag state feature."
          );
        }
      }
  
      EnforceDecimalHalfShape()
   
    public:
  
      SplitFields() noexcept : IntHalf{}, DecimalHalf{} {}
    };

    // Tail payload when MixedDec is enabled
    template<typename Policy>
    struct TailFields {
      using RestrictedFloatT = typename Policy::RestrictedFloatT;
      // MixedDec tail (RestrictedFloat default)
      RestrictedFloatT TrailingDigits{RestrictedFloatT::Zero};
      RestrictedFloatT TrailingAtZero{RestrictedFloatT::Zero}; // keep if you truly need it
      static inline constexpr bool HasTail = true;
    };
    
    // Empty tail when MixedDecMode is disabled
    struct NoTail {
      static inline constexpr bool HasTail = false;
    };
    
    template<typename Policy>
    struct CoreStorage: public std::conditional_t<Policy::SplitFieldsMode, SplitFields<Policy>, RawBlob<Policy>>,
    public std::conditional_t<HasTail, TailFields<Policy>, NoTail>
    {
      using Base = std::conditional_t<Policy::SplitFieldsMode,
      SplitFields<Policy>, RawBlob<Policy>>;
        
      using Tail = std::conditional_t<Tail::HasTail, TailFields<Policy>, NoTail>;

    public://Any publics from Raw blob and split fields that need IsZero() moved here
    
      // Check if zero
      constexpr bool IsZero() const noexcept {
        const bool headZero = [&]() constexpr {
          if constexpr (VariantName::SplitFieldsMode)
            return this->IntHalf==0&&this->DecimalHalf==0;
          else {
            if constexpr (Base::UnsignedMode && Base::SkipSafeCheck) {
              return this->raw == ZeroRaw;
            } else {
              return (this->raw & Base::Value_MASK) == Base::StoreT(0);
            }
          }
        }();

        if constexpr (HasTail) {
          return headZero && (this->TrailingDigits == this->TrailingAtZero);
        } else {
          return headZero;
        }
      }
    
      // -1, 0, +1 signum in semantic space (branchless form)
      constexpr int SignNum() const noexcept {
        if constexpr (Base::UnsignedMode) {
          return this->IsZero() ? 0 : 1;
        }
        //Assumes IntHalf exists and has comparison operations against signed int
        if constexpr (Base::SplitFieldsMode) {
          return IsZero() ? 0 : (this->IntHalf.IsNegative()?-1,1);
        }
        const int nz  = !this->IsZero(); // or however you compute nz
        const int bit = (((this->raw & Base::Sign_MASK) != 0) ? 1 : 0) ^ (Base::PositiveSign ^ 1);
        const int s   = (bit << 1) - 1;  // 1 → +1, 0 → -1
        return s & -nz;                  // mask to 0 if zero
      }

      // Unsigned: API sanity only — no-op and const is fine
      template<typename U = void, typename = std::enable_if_t<UnsignedMode, U>>
      constexpr inline void FixZeroSign() const noexcept {}

      // Signed: must mutate raw when at IsZero() if not correct sign
      template<typename U = void, typename = std::enable_if_t<!UnsignedMode, U>>
      inline void FixZeroSign() noexcept {
        if (IsZero()) {
          if constexpr (Policy::SplitFieldsMode) {
            //SplitFields: ensure IntHalf/Sign field is canonical positive
            //(fill in according to your IntHalf representation)
            //(MirroredInt already does this maintence)
          } else if constexpr (Policy::AllowUnneededWrites){
            // force sign bit to positive encoding
            if constexpr (Base::PositiveSign)
              raw |= Base::Sign_Mask;
            else 
              raw &= ~Base::Sign_Mask;
          } else
             if constexpr (Base::PositiveSign) {
             if ((this->raw & Base::Sign_MASK) == 0)  // only write if bit not set
               this->raw |= Base::Sign_MASK;
             } else {
             if ((this->raw & Base::Sign_MASK) != 0)  // only write if bit not clear
               this->raw &= ~Base::Sign_MASK;
             }
          }
        }
      }

      //Other internals that might need adjusting for MixedDec will be moved here if needed
    };
	};

  //The unified Storage type, layering Core + optional ExtraRep
  template<typename Policy>
  struct MDStorage: CoreStorage<Policy>,
  std::conditional_t<Policy::EnableExtraRep, ExtraRepFields<Policy>,NoExtraRep>
  {
      using Core      = CoreStorage<Policy>;
      using ExtraPart = std::conditional_t<Policy::EnableExtraRep&&Policy::EnableMaskedExtraRep==false,ExtraRepFields<Core>,NoExtraRep>;
  
      MDStorage() noexcept : Core(), ExtraPart() {}
  };

  template<class VariantClass, typename Policy:FixedDecDefaultPolicy>
  class MediumDecV3Base : protected MDStorage<VariantClass>
  {
  public:
    #pragma region DigitStorage
    //Digit storage created during MDStorageBase
    #pragma endregion DigitStorage

    #pragma region ValueSetters
    #pragma endregion ValueSetters
  };
}}