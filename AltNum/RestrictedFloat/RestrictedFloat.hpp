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
#include "../BlazesRusIntegerCode.hpp"//BlazesRusIntegerCode::IntPow(value,exp)
#include <bit>      // std::bit_width
#include <cstdint>  // uint32_t, uint64_t
#include <utility>  // std::pair

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

/// <summary>
/// Compact fractional representation used as TrailingDigits in MixedMode configurations of MediumDecV3Variant.
/// Encodes values in the range (0, ~1) as: 1 / (2^(exp + signif / MaxDenom))
/// - `exp` and `signif` are encoded fields; `MaxDenom` is a compile-time constant.
/// - Template parameters control precision, scaling, and layout.
/// - Default footprint: 4 bytes.
/// - zero represented by raw == 0
/// Primary purpose: extends precision beneath the main MixedDec value, recovering lost fractional detail.
/// Designed for deterministic, monotonic behavior across platforms.
/// Ideal for fixed-point math where sub-bit granularity is needed without floating-point drift.
/// </summary>
template<class Policy  = RFDefaultPolicy>
class DLL_API RestrictedFloat : RestrictedFloatBase,  {
protected:
  concept Has_EXP_BITS   = requires { { Policy::EXP_BITS }   -> std::convertible_to<unsigned>; };
  concept Has_SIG_BITS   = requires { { Policy::SIG_BITS }   -> std::convertible_to<unsigned>; };
  concept Has_NScaleFactor   = requires { { Policy::NScaleFactor }   -> std::convertible_to<unsigned>; };
  concept Has_AllowOne   = requires { { Policy::AllowOne }   -> std::convertible_to<unsigned>; };
  concept Has_ZeroSignifRep   = requires { { Policy::Has_ZeroSignifRep }   -> std::convertible_to<unsigned>; };
  // Optional: allow policy to override the exponent part of zero
  concept Has_ZeroExpRep = requires { { Policy::ZeroExpRep } -> std::convertible_to<unsigned>; };
public:

  using StoreT = std::conditional_t<requires { typename Policy::StoreT; },
                                    typename Policy::StoreT, u32>;

	//Intermediate needs to be twice StoreT to prevent overflow during certain parts of some potential operations
	using IntermediateT = std::conditional_t<requires { typename Policy::IntermediateT; }, typename Policy::IntermediateT,
			std::conditional_t<(sizeof(StoreT) <= 4),  // 32-bit or smaller
		  u64, unsigned __int128> // 64-bit StoreT needs 128-bit intermediate
	>;

  using DefaultOverflowT = std::conditional_t<requires { typename Policy::DefaultOverflowT; },
                                    typename Policy::DefaultOverflowT, u64>;														

  // Defaults: 7/25 split (total 32 bits), exp at top
  static inline constexpr unsigned EXP_BITS     = Has_EXP_BITS     ? Policy::EXP_BITS     : 7;
  static inline constexpr unsigned SIG_BITS     = Has_SIG_BITS     ? Policy::SIG_BITS     : 25;
  static inline constexpr unsigned NScaleFactor = Has_NScaleFactor ? Policy::NScaleFactor : 1;
	
	//If true then enables existance of 1.0 representation to be stored (Designed for standalone mode for sin ranges)
	//If enabled than zero is defined as some other number instead
  static inline constexpr bool AllowOne = Has_AllowOne ? Policy::AllowOne : false;

  // Derived limits
  static inline constexpr unsigned MaxDenom  = (SIG_BITS == 0 ? 1u : (1u << SIG_BITS));
  static inline constexpr unsigned SIG_MAX   = MaxDenom - 1;
  static inline constexpr unsigned EXP_MAX   = (EXP_BITS == 0 ? 0u : ((1u << EXP_BITS) - 1));

	static inline constexpr unsigned ZeroExpRep =
			AllowOne
					? (Has_ZeroExpRep ? Policy::ZeroExpRep : EXP_MAX) // default: peg exp at max in AllowOne mode
					: 0u;                                             // default: exp==0 in normal mode

	// Significand part of zero
	static inline constexpr unsigned ZeroSignifRep =
			AllowOne
					? (Has_ZeroSignifRep ? Policy::ZeroSignifRep : SIG_MAX)
					: 0u;

	// Now build the raw encoding for mathematical zero
	static inline constexpr StoreT ZeroRaw =
			(StoreT(ZeroExpRep) << SIG_BITS) | StoreT(ZeroSignifRep);

  // Masks (layout: [exp | signif])
  static inline constexpr StoreT SIG_MASK = (SIG_BITS == 0)
      ? StoreT{0}
      : (StoreT{1} << SIG_BITS) - StoreT{1};

  static inline constexpr StoreT EXP_MASK = ~SIG_MASK;

  // Static checks
  static_assert(std::is_unsigned_v<StoreT>,
                "StoreT must be an unsigned integral type");

  static_assert(EXP_BITS + SIG_BITS <= (sizeof(StoreT) * CHAR_BIT),
                "Policy::StoreT too narrow for configured bit-fields");

  static_assert(EXP_BITS + SIG_BITS > 0,
                "At least one of EXP_BITS or SIG_BITS must be > 0");

  // raw 32-bit storage
  StoreT raw;
  
  static constexpr unsigned ExpBits  = std::popcount(ExpMask);
  static constexpr unsigned ExpLSB   = std::countr_zero(ExpMask);
  static constexpr unsigned TotalBits  = sizeof(StoreT) * CHAR_BIT;


  constexpr RestrictedFloat() noexcept : raw(ZeroRaw) {}
  constexpr RestrictedFloat(unsigned exp, unsigned sig) noexcept 
  : raw((exp << SIG_BITS) | (sig & SIG_MAX)) 
  {
  // if somebody passed exp==0 and sig==0, stays zero
  // if exp>EXP_MAX or sig>SIG_MAX: caller error
  }

  constexpr RestrictedFloat(const StoreT& copy) noexcept : raw(copy) {}

  // extractors
  constexpr unsigned exp() const noexcept   { return raw >> SIG_BITS; }
  constexpr unsigned signif() const noexcept{ return raw & SIG_MAX; }
  constexpr bool   isZero() const noexcept { return raw_ == ZeroRaw; }

	// Smallest non-zero magnitude encoding
	static inline constexpr unsigned AlmostZeroSignif =
			AllowOne
					? (SIG_MAX - 1u) // leave SIG_MAX for ZeroRaw in AllowOne mode
					: SIG_MAX;

	static inline constexpr unsigned AlmostZeroExp = EXP_MAX;

	// Build the raw encoding
	static inline constexpr StoreT AlmostZeroRaw =
			(StoreT(AlmostZeroExp) << SIG_BITS) | StoreT(AlmostZeroSignif);

  // ----- partial-dec shuttle -----
  static inline constexpr unsigned QuarterMaxDenom   = MaxDenom/4;
  static inline constexpr unsigned ThreeFourthMaxDenom   = QuarterMaxDenom*3;
  static inline constexpr unsigned TwiceMaxDenom   = MaxDenom*2;

	// Zero fast path
	inline void SetAsZero() noexcept {
			raw = ZeroRaw;
	}

	// Set only exp; signif preserved unless clamped by policy.
	inline void SetExp(unsigned newExp) noexcept {
			unsigned e = (newExp > EXP_MAX) ? EXP_MAX : newExp;
			unsigned s = signif(); // current

			raw = (StoreT(e) << SIG_BITS) | StoreT(s & SIG_MAX);
	}

	// Set only signif; exp preserved, but may be clamped by policy at MAX.
	inline void SetSignif(unsigned newSignif) noexcept {
			unsigned e = exp(); // current
			unsigned s = newSignif & SIG_MAX;

			raw = (StoreT(e) << SIG_BITS) | StoreT(s);
	}

	// Set both together in one shot (preferred to avoid transient invalid states).
	inline void SetExpSignif(unsigned newExp, unsigned newSignif) noexcept {
			// Clamp to field widths first
			if (newExp > EXP_MAX) {
					raw = AlmostZeroRaw;
					return;
			}
			if (newSignif > SIG_MAX) {
					// If we're already at EXP_MAX, this means smaller than smallest non-zero
					if (newExp == EXP_MAX) {
							raw = AlmostZeroRaw;
							return;
					}
					newSignif = SIG_MAX;
			}

			// If exactly mathematical zero, set ZeroRaw
			if (newExp == ZeroExpRep && newSignif == ZeroSignifRep) {
					raw = ZeroRaw;
					return;
			}

			// Normal store
			raw = (StoreT(newExp) << SIG_BITS) | StoreT(newSignif);
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
    auto e = exp();
    auto s = signif();
    // Case: Exp=0, Signif=0 -> exact zero
    if (e == 0 && signif() == 0)
      return "0";

    // Zero band (Exp=0, Signif != 0)
    if (e == 0)
      return ZeroBandToString(s, fracBits, static_cast<u32>(maxDigits));

    // General bands: normal (<EXP_MAX) + subnormal (>=EXP_MAX)
    return ToStringImpl(exp, s, fracBits, static_cast<u32>(maxDigits));
  }

  // Sign-aware operation dispatch for MixedDec (Exp, SignifNum noted in comments).
  // - `Op*` prefix: in-place mutation of this object.
  // - `By*` prefix: returns a modified copy, leaving this object unchanged.
  // All operations respect the current sign convention and MixedMode layout.
  // Used for arithmetic that must account for Exp/SignifNum fields and TrailingDigits precision.

  #pragma region Floating Conversion

  // Policy-aware limits
  static constexpr double MaxValue = AllowOne ? 1.0
   : std::pow(2.0, -(0.0 + 1.0 / static_cast<double>(MaxDenom)));
  
  static constexpr double MinValue =
      std::pow(2.0, -(static_cast<double>(MaxExp) +
      (static_cast<double>(MaxDenom) - 1.0) / static_cast<double>(MaxDenom)));
  
  constexpr float ToFloat(const RTable<SIG_BITS>& rt) const noexcept {
      uq64 fracQ = rt.pow2_neg_frac(signif);
      uq64 valQ  = (exp >= Q) ? (fracQ >> exp) : (fracQ << (Q - exp));
      return static_cast<float>(valQ) / static_cast<float>(Q_ONE);
  }
  
  static constexpr RestrictedFloat FromFloat(float value, const RTable<SIG_BITS>& rt) {
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
  
  constexpr double ToDouble(const RTable<SIG_BITS>& rt) const noexcept {
      uq64 fracQ = rt.pow2_neg_frac(signif);
      uq64 valQ  = (exp >= Q) ? (fracQ >> exp) : (fracQ << (Q - exp));
      return static_cast<double>(valQ) / double(Q_ONE);
  }
  
  static constexpr RestrictedFloat FromDouble(double value, const RTable<SIG_BITS>& rt) {
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
  if (signif()!=that.signif())
    return false;
  if (exp()!=that.exp())
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
      const u128 D   = u128{1} << SIG_BITS;
      const u128 twoD  = D << 1;

      auto set_zero_band = [&](u128 numer, u128 denom) {
      // s = round( (1 - numer/denom) * 2D )
      u128 t = (denom > numer) ? (denom - numer) : u128{0};
      u128 s128 = (t * twoD + (denom>>1)) / denom; // round half-up
      if (s128 == 0) s128 = 1;           // avoid reserved zero sentinel in zero band
      if (s128 > twoD - 1) s128 = twoD - 1;    // clamp
      raw = (StoreT(0) << SIG_BITS) | StoreT(s128 & SIG_MASK);
      };

      auto set_general_band = [&](u128 numer, u128 denom) {
      // We want x = v/scale ≈ (2D - s) / (D * 2^(exp+1))
      // => (2D - s) ≈ x * D * 2^(exp+1) = (numer * D << (exp+1)) / denom
      // Pick exp so that (2D - s) is in [1,2D] and s in [0,2D-1].
      // Start with exp = 0 and grow until within range or capped.
      u32 e = 0;
      u128 target = 0;
      while (e < EXP_MAX) {
        u128 scaled = ( (numer * D) << (e + 1) ) / denom; // floor
        if (scaled == 0) { ++e; continue; }
        if (scaled <= twoD) { target = scaled; break; }
        ++e;
      }
      if (target == 0) {
        // Too small even at EXP_MAX -> underflow to zero(only when reading from string not in normal operation)
        raw = ZeroRaw;
        return;
      }
      // s = 2D - target
      u128 s128 = twoD - target;
      if (s128 > twoD - 1) s128 = twoD - 1; // clamp
      raw = (StoreT(e) << SIG_BITS) | StoreT( static_cast<StoreT>(s128) & SIG_MASK );
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

  // Sign-aware operation dispatch for MixedDec (Exp, SignifNum noted in comments).
  // - `Op*` prefix: in-place mutation of this object.
  // - `By*` prefix: returns a modified copy, leaving this object unchanged.
  // All operations respect the current sign convention and MixedMode layout.
  // Used for arithmetic that must account for Exp/SignifNum fields and TrailingDigits precision.
  // DivOp and MultOp functions unlikely to be used by the MixedDec operators
  // While AddOp and SubOp likely to be used by MixedDec
  // 
  #pragma region MixedDec Operator Helpers

	template<UnsignedIntegerType IntType=unsigned int>
	constexpr std::pair<unsigned, unsigned>
	IntToPowerFields(IntType val) noexcept {
			// Integer exponent = index of top bit
			unsigned exp = std::bit_width(val) - 1;

			// Remainder after removing top bit
			IntType remainder = val - (IntType(1) << exp);

			// Fractional exponent numerator scaled to SignifBits
			unsigned signif = unsigned((remainder << SIG_BITS) >> exp);

			// Normalise if fractional part overflows
			if (signif == (1u << SIG_BITS)) {
					signif = 0;
					++exp;
			}
			//Result will be in Positive exponent range but fine for temporary conversion for unsigned division and multiplication
			return {exp, signif};
	}

	//Treating as 1/2^(this->exp()+this->signif()/MaxDenom) * 1/(2^tempExp+tempSignif/MaxDenom)
  template<UnsignedIntegerType IntType=unsigned int, typename OverflowT=IntType>
  inline void UIntDivOp(const IntType& rhs){
	  auto [tempExp, tempSignif] = IntToPowerFields(rhs);
	  tempSignif += signif();
		if(tempSignif>MaxDenom){
		  tempSignif -= MaxDenom;
			tempExp++;
		}
		tempExp += exp();
		SetExpSignif(tempExp, tempSignif);
		//No danger of overflow with integer division
  }


	// After your IntToPowerFields(rhs) gives (rhsExp, rhsSignif), do:
	// tempExp/tempFrac are in inverted form
	inline void normalize_signed(signed& e, signed& f) {
			if constexpr (SIG_BITS != 0) {
					if (f < 0) { f += signed(MaxDenom); --e; }
					else if (f >= signed(MaxDenom)) { f -= signed(MaxDenom); ++e; }
			} else {
					f = 0;
			}
	}

	// Convert inverted (tempExp,tempFrac) to normal-space Q-fixed using r-table
	template<unsigned SIG_BITS>
	inline uq64 inverted_to_Q(signed tempExp, unsigned tempFrac, const RTable<SIG_BITS>& tbl) {
			uq_t val = tbl.pow2_neg_frac(tempFrac); // 2^(-tempFrac/Den) in Q
			if (tempExp >= 0) {
					unsigned s = static_cast<unsigned>(tempExp);
					return (s >= 127 ? 0 : static_cast<uq64>(val >> std::min(127u, s)));
			} else {
					unsigned s = static_cast<unsigned>(-tempExp);
					return (s >= 63 ? 0 : static_cast<uq64>(val << std::min(63u, s)));
			}
	}

	//Treating as 1/2^(this->exp()+this->signif()/MaxDenom) * (2^tempExp+tempSignif/MaxDenom)
  template<UnsignedIntegerType IntType=unsigned int, typename OverflowT=IntType>
  inline OverflowT UIntMultOp(const IntType& rhs){
    // classic form
	  auto [rhsExp, rhsSignif] = IntToPowerFields(rhs);
    //Inverted form
    auto lhsExp = exp();
    auto lhsSignif = signif();
    //Both are reused for result magnitude
    if(rhsExp>=lhsExp){
      //If rhs magnitude is equal or greator than the inverted form from lhs, then value has overflowed
      if(rhsSignif>lhsSignif){
        // Reusing rhsExp/rhsSignif as relative magnitude delta after multiply
        rhsExp -= lhsExp;// exponent delta
        rhsSignif -= lhsSignif;// fractional delta
        // Integer portion from exponent delta
        const IntType scale = IntType(1) << rhsExp;
        OverflowT carry = OverflowT(scale);

        // Add integer portion from fractional delta
        const OverflowT mixed = OverflowT(scale) * OverflowT(rhsSignif);
        carry += mixed / MaxDenom;

        // Remainder drives new inverted-form fractional
        const IntType rem = static_cast<IntType>(mixed % MaxDenom);
        if (rem == 0) {
            SetAsZero();
        } else {
            // Remaining classic fraction = rem/MaxDenom
            // In inverted form, that's a "gap" of MaxDenom - rem at exp=0
            SetExpSignif(0, MaxDenom - rem);
        }

        return carry;
      } else if(rhsSignif==lhsSignif){
        //Overflow detected but code path is simplified because no signif or TrailingDigits left()
        rhsExp -= lhsExp;// exponent delta
        SetAsZero();
        return IntType(1) << rhsExp;
      } else {
        //No Overflow Detected(Values reused for result)
        lhsExp -= rhsExp;// exponent delta
        lhsSignif -= rhsSignif;// fractional delta
        SetExpSignif(lhsExp, lhsSignif);
      }
    } else {//No Overflow Detected
      if (lhsSignif >= rhsSignif) {
        // No borrow from exponent
        SetExpSignif(lhsExp, lhsSignif - rhsSignif);
      } else {
        // Borrow 1 from exponent: fractional = MaxDenom - (rhsSignif - lhsSignif)
        const IntType gap = rhsSignif - lhsSignif; // 1..MaxDenom-1
        SetExpSignif(lhsExp - 1, MaxDenom - gap);
      }
    }
		return 0u;
  }

  template<typename OverflowT=DefaultOverflowT>
  inline OverflowT DivOp(const RestrictedFloat &rhs){
    auto lhsExp    = exp();
    auto lhsSignif = signif();
    auto rhsExp    = rhs.exp();
    auto rhsSignif = rhs.signif();

    if (lhsExp >= rhsExp) {
        if (lhsSignif > rhsSignif) {
            // delta = (lhsExp - rhsExp, lhsSignif - rhsSignif)
            lhsExp    -= rhsExp;    // exponent delta
            lhsSignif -= rhsSignif; 
            const IntType scale = IntType(1) << lhsExp;
            OverflowT carry = OverflowT(scale);
            const OverflowT mixed = OverflowT(scale) * OverflowT(lhsSignif);
            carry += mixed / MaxDenom;
            const IntType rem = static_cast<IntType>(mixed % MaxDenom);
            if (rem == 0) SetAsZero();
            else SetExpSignif(0, MaxDenom - rem);
            return carry;
        }
        else if (lhsSignif == rhsSignif) {
            lhsExp -= rhsExp;
            SetAsZero();
            return IntType(1) << lhsExp;
        }
        else {
            lhsExp    -= rhsExp;
            rhsSignif -= lhsSignif; // borrow-safe if you normalise
            SetExpSignif(lhsExp, rhsSignif);
        }
    } else {
        // no overflow, borrow-safe subtraction
        rhsExp -= lhsExp;
        if (rhsSignif >= lhsSignif) {
            SetExpSignif(rhsExp, rhsSignif - lhsSignif);
        } else {
            const IntType gap = lhsSignif - rhsSignif;
            SetExpSignif(rhsExp - 1, MaxDenom - gap);
        }
    }
    return OverflowT(0);
  }
  
	template<typename StoreT=DefaultOverflowT>
  inline void MultOp(const RestrictedFloat &rhs){
    auto sumExp    = exp()    + rhs.exp();
    auto sumSignif = signif() + rhs.signif();
    if (sumSignif >= MaxDenom) { sumSignif -= MaxDenom; ++sumExp; }
    SetExpSignif(sumExp, sumSignif);
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
      uint32_t signifPart = 0;
      if (denomRem != numRem) {
          // fracPow = log2(denomRem) - log2(numRem)
          int fracPow = int(std::countr_zero(denomRem)) - int(std::countr_zero(numRem));
          signifPart  = uint32_t(fracPow)*OV;
      }

			SetExpSignif(uint32_t(expPart), signifPart);
  }
  
  // For the fractional-exponent band: convert from an exact power-of-two ratio
  // numerator/denominator back to (exp, signif) where
  // value = 1 / 2^(exp + signif / MaxDenom)
  template<typename TickT = StoreT>
  inline void ConvertFracPair(TickT numerator, TickT denominator)
  {
      constexpr TickT OV = MaxDenom; // fractional ticks per whole exponent step

      TickT ratioNum = denominator;
      TickT ratioDen = numerator;

      int32_t expPart = 0;
      while (ratioNum >= (ratioDen << 1)) { ratioNum >>= 1; ++expPart; }
      while (ratioNum < ratioDen)         { ratioNum <<= 1; --expPart; }

      uint32_t signifPart = 0;
      if (ratioNum != ratioDen) {
          int fracPow = int(std::countr_zero(ratioNum)) - int(std::countr_zero(ratioDen));
          signifPart  = uint32_t(fracPow) * OV;
      }

			SetExpSignif(uint32_t(expPart), signifPart);
  }
  
  // Returns: 0 = no coarse change, 1 = promote +1 coarse unit, 2 = borrow -1 coarse unit
  template<typename ValueT, typename TickT=StoreT>
  inline uint8_t TailAddSubInPlace(const RestrictedFloat& rhsTail,
  bool leftIsPositive=true,bool rightIsPositive=true)
  {
    auto lExp = this-> exp();
    auto rExp = rhsTail.exp();
    auto lSignif = this->signif();
    auto rSignif = rhsTail.signif();
    bool sameSign = leftIsPositive==rightIsPositive;
    // Align smaller-exp side to larger-exp side
    if (lSignif == 0 && rSignif == 0) {
      //Shift numerator to same denom such as 1/2 + 1/4 becomes 2/4 + 1/4
      //Shift numerator to same denom such as 1/2 + 1/8 becomes 4/8 + 1/8
      //Shift numerator to same denom such as 1/2 - 1/4 becomes 2/4 - 1/4
      //Shift numerator to same denom such as 1/8 - 1/2 becomes 1/8 - 4/8 = -3/8
      if (lExp < rExp) {
        auto shift = rExp - lExp;
        StoreT numResult = sameSign ? (1 << shift) + 1: (1 << shift) - 1:
        ConvertPureExpPair(numResult, rExp);
        if(sameSign||rightIsPositive) return 0;//1/2 - 1/4 becomes 2/4 - 1/4
        else
          return 2;
      } else if (rExp < lExp) {
        auto shift = lExp - rExp;
        StoreT numResult = sameSign ? (1 << shift) - 1: (1 << shift) + 1:
        ConvertPureExpPair(numResult, lExp);
        if(sameSign) return 0;
        else if(rightIsPositive)//-1/8 + 1/2 becomes -1/8 + 4/8 = 3/8
          return 1;
        else
          return 2;
      } else if(leftIsPositive==rightIsPositive)//Doubling
          //(exp:-1) 0.5 + 0.5 = (exp:0) 1.0
          //(exp:-2) 0.25 + 0.25 = (exp:-1) 0.5
          //(exp:-3) 0.0.125 + 0.0.125 = (exp:-1) 0.25
          if(lExp==1){
            SetAsZero();
            return 1;
          }
          SetExp(lExp-1);
          return 0;
        } else {//canceling tail
          SetAsZero();
          return 0;
        }
      }
    } else {
        // Fractional-exponent band: at least one signif > 0
        // We'll align the smaller mixed exponent to the larger one.

        constexpr TickT OV = MaxDenom; // fractional ticks per whole exponent step

        auto lessMixed = [&](uint32_t e1, uint32_t s1, uint32_t e2, uint32_t s2) noexcept {
            return (e1 < e2) || (e1 == e2 && s1 < s2);
        };

        auto mixedDiff = [&](uint32_t eA, uint32_t sA,
                             uint32_t eB, uint32_t sB,
                             uint32_t& dE, uint32_t& dF) noexcept {
            // Borrow from integer exponent if needed
            if (sA > sB) {
                dE = (eB - eA) - 1;
                dF = (sB + OV) - sA;
            } else {
                dE = (eB - eA);
                dF = sB - sA;
            }
        };

        TickT lhsNum = 1;
        TickT rhsNum = 1;
        uint32_t tgtExp, tgtSig;

        if (lessMixed(lExp, lSignif, rExp, rSignif)) {
            // LHS has smaller mixed exponent → align to RHS
            uint32_t dE, dF; mixedDiff(lExp, lSignif, rExp, rSignif, dE, dF);
            if (dE) lhsNum <<= dE;
            if (dF) lhsNum = TickT((uStoreT(lhsNum) * dF) / OV);
            tgtExp = rExp; tgtSig = rSignif;
        } else if (lessMixed(rExp, rSignif, lExp, lSignif)) {
            // RHS has smaller mixed exponent → align to LHS
            uint32_t dE, dF; mixedDiff(rExp, rSignif, lExp, lSignif, dE, dF);
            if (dE) rhsNum <<= dE;
            if (dF) rhsNum = TickT((uStoreT(rhsNum) * dF) / OV);
            tgtExp = lExp; tgtSig = lSignif;
        } else {
            // Already same mixed exponent
            tgtExp = lExp; tgtSig = lSignif;
        }

        // Add/sub numerators
        StoreT resultNum = sameSign
                          ? StoreT(lhsNum) + StoreT(rhsNum)
                          : StoreT(lhsNum) - StoreT(rhsNum);

        // Detect coarse carry/borrow: in fractional band, one coarse unit = OV ticks
        uint8_t code = 0;
        if (resultNum >= StoreT(OV)) {
            resultNum -= StoreT(OV);
            code = 1;
        } else if (resultNum < 0) {
            resultNum += StoreT(OV);
            code = 2;
        }

        if (resultNum == 0) {
            SetAsZero();
            return code;
        }

        // Convert back to (exp, signif)
        ConvertFracExpPair(TickT(resultNum), OV); // OV is the fractional denominator
        return code;
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
