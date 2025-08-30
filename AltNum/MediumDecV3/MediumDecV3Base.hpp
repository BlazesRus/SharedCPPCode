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
    //Both bitwise and RawBlobe share features here
    struct GlobalStorage {
    protected:
      static inline constexpr bool PositiveSign   = Policy::PositiveSign;

      static inline constexpr bool EnableStateFlags   =
      Policy::EnablePi || Policy::EnableE || Policy::EnableI;

      using IntHalfT   = typename Policy::IntHalfT;
      using DecimalHalfT = typename Policy::DecimalHalfT;

    };
    // when bitwise mode:
    struct RawBlob : GlobalStorage {
    //IntValue is magnitude of Integer Half even when negative
    protected:
      using StoreT  = typename Policy::StoreT;

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

    protected:
      using StoreT  = typename Policy::StoreT;

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

      //Bitlayout:Sign(SIGN_BITS Bits)->Value(IntHalf(INT_BITS Bits)+DecimalHalf(DECIMAL_BITS Bits))
      //->Flag State(FLAG_BITS Bits)->Masked ExtraRep(EXTRA_BITS Bits)->AltRepFlag(s)(ALTREP_BITS Bits)

      // --- Effective flags ---
      static inline constexpr bool UnsignedMode     = Policy::UnsignedMode;
      static inline constexpr bool DisableBitwiseMaskMode = Policy::DisableBitwiseMaskMode;

      static inline constexpr bool EnableMaskedExtraRep =
        Policy::EnableMaskedExtraRep && !Policy::DisableBitwiseMaskMode;
      static inline constexpr bool EnableExtraRep =
        Policy::EnableExtraRep || Policy::EnableMaskedExtraRep;

      // --- Widths ---
      static inline constexpr unsigned SIGN_BITS  = UnsignedMode ? 0u : 1u;
      static inline constexpr unsigned INT_BITS   = Policy::IntValue_BITS;
      static inline constexpr unsigned DECIMAL_BITS = Policy::DecimalHalf_BITS;
      static inline constexpr unsigned VALUE_BITS   = INT_BITS + DECIMAL_BITS;
      static inline constexpr unsigned FLAG_BITS  = EnableStateFlags ? Policy::FlagState_BITS : 0u;
      static inline constexpr unsigned EXTRA_BITS   = EnableMaskedExtraRep ? Policy::MaskedExtra_BITS : 0u;
    
      // --- Shifts (Sign on top) ---
      static inline constexpr unsigned SHIFT_SIGN  = INT_BITS + DECIMAL_BITS + FLAG_BITS + EXTRA_BITS;
      static inline constexpr unsigned SHIFT_INT   = DECIMAL_BITS + FLAG_BITS + EXTRA_BITS;
      static inline constexpr unsigned SHIFT_DECIMAL = FLAG_BITS + EXTRA_BITS;
      static inline constexpr unsigned SHIFT_VALUE  = SHIFT_DECIMAL; // low end starts at DecimalHalf
      static inline constexpr unsigned SHIFT_FLAG  = EXTRA_BITS;
      static inline constexpr unsigned SHIFT_EXTRA   = 0u;

      //Static storage-width check
      static_assert(
      (SIGN_BITS + INT_BITS + DecimalHalf_BITS + FlagState_BITS + EXTRA_BITS) <= (sizeof(StoreT)*CHAR_BIT),
      "Policy::StoreT too narrow for configured bit-fields"
      );

      // --- PURE masks ---
      static inline constexpr StoreT PURE_SIGN_MASK  = ones(SIGN_BITS);
      static inline constexpr StoreT PURE_INT_MASK   = ones(INT_BITS);
      static inline constexpr StoreT PURE_DEC_MASK   = ones(DECIMAL_BITS);
      static inline constexpr StoreT PURE_VALUE_MASK = ones(VALUE_BITS);
      static inline constexpr StoreT PURE_FLAG_MASK  = ones(FLAG_BITS);
      static inline constexpr StoreT PURE_EXTRA_MASK = ones(EXTRA_BITS);

      // --- Shifted masks ---
      static inline constexpr StoreT Sign_MASK  = PURE_SIGN_MASK  << SHIFT_SIGN;
      static inline constexpr StoreT Int_MASK   = PURE_INT_MASK   << SHIFT_INT;
      static inline constexpr StoreT Dec_MASK   = PURE_DEC_MASK   << SHIFT_DECIMAL;
      static inline constexpr StoreT Value_MASK    = PURE_VALUE_MASK << SHIFT_VALUE;
      static inline constexpr StoreT Flag_MASK  = PURE_FLAG_MASK  << SHIFT_FLAG;
      static inline constexpr StoreT Extra_MASK   = PURE_EXTRA_MASK << SHIFT_EXTRA;

      // --- Clamp helpers ---
      static constexpr unsigned clampSign(unsigned v)  noexcept { return unsigned(v & unsigned(PURE_SIGN_MASK)); }
      static constexpr unsigned clampInt(unsigned v)   noexcept { return unsigned(v & unsigned(PURE_INT_MASK)); }
      static constexpr unsigned clampDec(unsigned v)   noexcept { return unsigned(v & unsigned(PURE_DEC_MASK)); }
      static constexpr unsigned clampValue(unsigned v) noexcept { return unsigned(v & unsigned(PURE_VALUE_MASK)); }
      static constexpr unsigned clampFlags(unsigned v) noexcept { return unsigned(v & unsigned(PURE_FLAG_MASK)); }
      static constexpr unsigned clampExtra(unsigned v) noexcept { return unsigned(v & unsigned(PURE_EXTRA_MASK)); }

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

      static constexpr unsigned splitIntPart(StoreT value) noexcept {
        return unsigned((value >> DECIMAL_BITS) & PURE_INT_MASK);
      }
      static constexpr unsigned splitDecPart(StoreT value) noexcept {
        return unsigned(value & PURE_DEC_MASK);
      }
      
      static constexpr StoreT mergeParts(unsigned intPart, unsigned decPart) noexcept {
        return ((StoreT(intPart) & PURE_INT_MASK) << DECIMAL_BITS) |
           (StoreT(decPart) & PURE_DEC_MASK);
      }

      static constexpr StoreT assemble(unsigned sign, unsigned i, unsigned d, unsigned f, unsigned e=0) noexcept {
        return packSign(sign) | packInt(i) | packDec(d) | packFlag(f) | packExtra(e);
      }

      static constexpr StoreT assembleValue(unsigned i, unsigned d, unsigned e=0) noexcept {
        return packInt(i) | packDec(d) | packExtra(e);
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

      constexpr unsigned Value() const noexcept {
        return unsigned((raw & Value_MASK) >> SHIFT_VALUE);
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
        if constexpr ((UnsignedMode)) return true;
        if constexpr (PositiveSign==1) return (raw & Sign_MASK) == 1;
        else return (raw & Sign_MASK) == 0;
      }
      
      constexpr bool IsNegative() const noexcept {
        if constexpr ((UnsignedMode)) return false;
        if constexpr (PositiveSign==1) return (raw & Sign_MASK) == 0;
        else return (raw & Sign_MASK) == 1;
      }

      void magSubOrFlip(const ValueT& rhs) noexcept {
          if (rhs > Value()) {
              Value() = rhs - Value();
              FlipSign();
          } else {
              Value() -= rhs;
          }
      }

      void Bitwise_UnsignedAddOp(const ValueT& r) const noexcept {
        if constexpr ((UnsignedMode))
          Value() += r;
        else {
          if(IsPositive())
            Value() += r;
          else
            magSubOrFlip(r);
        }
      }

      void Bitwise_AddOp(const ValueT& r, const bool rightIsPositive=true) const noexcept {
        if constexpr (UnsignedMode)
          Value() += r;
        else {
          if(IsPositive()){
            if(rightIsPositive)
              Value() += r;
            else
              magSubOrFlip(r);
          } else {
            if(rightIsPositive)
              magSubOrFlip(r);
            else
              Value() += r;
          }
        }
      }

      void Bitwise_UnsignedSubOp(const ValueT& r) const noexcept {
        if constexpr ((UnsignedMode))
          Value() -= r;
        else {
          if(IsPositive()){
            magSubOrFlip(r);
          }
          else
            Value() += r;
        }
      }

      void Bitwise_SubOp(const ValueT& r, const bool rightIsPositive=true) const noexcept {
        if constexpr ((UnsignedMode))
          Value() -= r;
        else {
          if(IsPositive()){
            if(rightIsPositive)
              magSubOrFlip(r);
            else
              Value() += r;
          } else {
            if(rightIsPositive)
              Value() += r;
            else
              magSubOrFlip(r);
          }
        }
      }

      void Bitwise_UIntMultOp(const ValueT& r, bool rightIsPositive=true) const noexcept {
      }

      void Bitwise_UIntDivOp(const ValueT& r, bool rightIsPositive=true) const noexcept {

      }
			
      void Bitwise_IntMultOp(const ValueT& r, bool rightIsPositive=true) const noexcept {
      }

      void Bitwise_IntDivOp(const ValueT& r, bool rightIsPositive=true) const noexcept {

      }

      void Bitwise_UnsignedMultOp(const ValueT& r, bool rightIsPositive=true) const noexcept {
      }

      void Bitwise_UnsignedDivOp(const ValueT& r, bool rightIsPositive=true) const noexcept {

      }
			
      void Bitwise_MultOp(const ValueT& r, bool rightIsPositive=true) const noexcept {
      }

      void Bitwise_DivOp(const ValueT& r, bool rightIsPositive=true) const noexcept {

      }

      //BitwiseExtended versions catch the lost digits
      constexpr ValueT BitwiseExtended_MultOp(const ValueT& r, bool rightIsPositive=true) const noexcept {
      }

      constexpr ValueT BitwiseExtended_DivOp(const ValueT& r, bool rightIsPositive=true) const noexcept {

      }

    };

    // when split mode:
    struct SplitFields : GlobalStorage {
    public:
      IntHalfT   IntHalf;
      DecimalHalfT DecimalHalf;

      //Checking if DecimalHalf is supported type before using table
      static constexpr DecimalHalfT DecimalOverflow = []{
        if constexpr (has_overflow_limit<Policy>::value) {
          return static_cast<DecimalHalfT>(Policy::DecimalOverflow);
        } else if constexpr (std::is_integral_v<DecimalHalfT>) {
          return static_cast<DecimalHalfT>(
            VariableConversionFunctions::PowerOfTens64Bit[Policy::DecimalHalfDigits + 1]
          );
        } else if constexpr (CheckMemberOrMethodExistance::HasValueMember<DecimalHalfT>::value) {
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
    struct CoreStorage: public std::conditional_t<Policy::DisableBitwiseMaskMode, SplitFields<Policy>, RawBlob<Policy>>,
    public std::conditional_t<HasTail, TailFields<Policy>, NoTail>
    {
      using Base = std::conditional_t<Policy::DisableBitwiseMaskMode,
      SplitFields<Policy>, RawBlob<Policy>>;
        
      using Tail = std::conditional_t<Tail::HasTail, TailFields<Policy>, NoTail>;

      //Prevent drift of policy later(and enable debugging of linked policy constants later--other policies are stored to constexpr earlier)
      static inline constexpr bool DisableBitwiseMaskMode = Policy::DisableBitwiseMaskMode;

    protected:
      static inline constexpr StoreT ZeroRaw = []() constexpr {
        if constexpr (Policy::UnsignedMode) {
          return Base::StoreT(0);
        } else {
          return (Base::StoreT(0) & Base::Value_MASK)
          | (Base::PositiveSign ? Base::Sign_Mask : Base::StoreT(0));
        }
      }();

    public://Any publics from Raw blob and split fields that need IsZero() moved here
    
      // Check if zero
      constexpr bool IsZero() const noexcept {
        const bool headZero = [&]() constexpr {
          if constexpr (Base::UnsignedMode && Policy::SkipSafeCheck) {
            return this->raw == ZeroRaw;
          } else {
            return (this->raw & Base::Value_MASK) == Base::StoreT(0);
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
        if constexpr (Base::DisableBitwiseMaskMode) {
          return IsZero() ? 0 : (this->IntHalf<0?-1,1);
        }
        if constexpr (Base::PositiveSign) {//PositiveSign is when Positive sign defined as 1(Bit definition for 1)
          const int bit = ((this->raw & Base::Sign_MASK) ? 1 : 0);
          const int s   = (bit << 1) - 1;   // 1->+1, 0->-1
          return s & -nz;                   // mask to 0 if zero
        } else {
          const int bit = ((this->raw & Base::Sign_MASK) == 0 ? 1 : 0);
          const int s   = (bit << 1) - 1;   // 1->+1, 0->-1
          return s & -nz;
        }
      }

      // Unsigned: API sanity only — no-op and const is fine
      template<typename U = void, typename = std::enable_if_t<UnsignedMode, U>>
      constexpr inline void FixZeroSign() const noexcept {}

      // Signed: must mutate raw when at IsZero() if not correct sign
      template<typename U = void, typename = std::enable_if_t<!UnsignedMode, U>>
      inline void FixZeroSign() noexcept {
        if (IsZero()) {
          if constexpr (Policy::DisableBitwiseMaskMode) {
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
      using ExtraPart = std::conditional_t<Policy::EnableExtraRep,ExtraRepFields<Policy>,NoExtraRep>;
  
      MDStorage() noexcept : Core(), ExtraPart() {}
  };

  template<class VariantClass, typename Policy:FixedDecDefaultPolicy>
  requires FixedDecPolicyVariant<Policy>
  class MediumDecV3Base : protected MDStorage<VariantClass>
  {
  public:
    #pragma region DigitStorage
    //Digit storage created during MDStorageBase
    #pragma endregion DigitStorage

    #pragma region ValueSetters
/*
    inline void SetIntHalf(IntHalfT& v const) noexcept {
      if constexpr(Policy::DisableBitwiseMaskMode) {
      } else {
      }
    }

    inline void SetIntHalfFromSigned(IntHalfSignedT& v const) noexcept {
      if constexpr(Policy::DisableBitwiseMaskMode) {
      } else {
      }
    }

    inline void SetDecimalHalf(DecimalHalfT& v const) noexcept {
      if constexpr(Policy::DisableBitwiseMaskMode) {
			  DecimalHalf = v; 
      } else {
			  DecimalHalf 
      }
    }

    inline void SetExtraRep(ExtraRepT& v const) noexcept {
      if constexpr(Policy::DisableBitwiseMaskMode) {
      } else {
      }
    }

    inline void SetFlags(unsigned& v const) noexcept {
      if constexpr(Policy::DisableBitwiseMaskMode) {
      } else {
      }
    }

    template<typename = std::enable_if_t<!Policy::DisableBitwiseMaskMode>
    inline void SetMagnitude(unsigned v) noexcept {
      Value() = v;
    }
*/
    #pragma endregion ValueSetters
  };
}}