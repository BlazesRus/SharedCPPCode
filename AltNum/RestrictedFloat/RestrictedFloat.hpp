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
/// Determinative 0 to Almost 1 representation
/// Use template variables to adjust behavior
/// (Reserves 4 bytes of storage)
/// </summary>
template<class Policy  = RFDefaultPolicy>
class DLL_API RestrictedFloat : RestrictedFloatBase,  {
  static_assert(RFPolicyVariant<Policy>,
  "Policy must satisfy RFPolicyVariant");

public:

  // only 32 bits total (exp needs to be at top of bitfields
  // so that if EnabledSubnormals is enabled can compare if raw_ is greater than normal range band
  // Or optionally allow partial values in decreases of values if can fit in calculations
  //  7 bits exponent  [0..127]
  // 25 bits significand [0..2^25-1]
  static inline constexpr unsigned EXP_BITS   = expBits;
  static inline constexpr unsigned SIG_BITS   = fracBits;
  static inline constexpr unsigned SIG_MAX  = (1u << SIG_BITS) - 1;
  static inline constexpr unsigned EXP_MAX  = (1u << EXP_BITS) - 1;
  // zero represented by raw == 0
  // special band at Exp==0 and Signif>0: value = .5*(Signif+DenomMax)/DenomMax
  static inline constexpr unsigned DenomMax   = 1u << SIG_BITS;
  static inline constexpr unsigned ZeroRaw  = 0u;

  // Bit layout: [exp | signif] in StoreT
  static inline constexpr StoreT SIG_MASK   = (SIG_BITS == 0) ? StoreT{0} : (StoreT{1} << SIG_BITS) - 1;
  static inline constexpr StoreT EXP_MASK   = StoreT{0} - StoreT{1}; // all-ones

  // raw 32-bit storage
  StoreT raw_;
  
  static constexpr unsigned ExpBits  = std::popcount(ExpMask);
  static constexpr unsigned ExpLSB   = std::countr_zero(ExpMask);
  static constexpr unsigned TotalBits  = sizeof(StoreT) * CHAR_BIT;


  constexpr RestrictedFloat() noexcept : raw_(ZeroRaw) {}
  constexpr RestrictedFloat(unsigned exp, unsigned sig) noexcept 
  : raw_((exp << SIG_BITS) | (sig & SIG_MAX)) 
  {
  // if somebody passed exp==0 and sig==0, stays zero
  // if exp>EXP_MAX or sig>SIG_MAX: caller error
  }
 constexpr RestrictedFloat(const StoreT& copy) noexcept : raw_(copy) {}

  // extractors
  constexpr unsigned exp() const noexcept   { return raw_ >> SIG_BITS; }
  constexpr unsigned signif() const noexcept{ return raw_ & SIG_MAX; }
  constexpr bool   isZero() const noexcept { return raw_ == ZeroRaw; }

  // ----- partial-dec shuttle -----
  static inline constexpr unsigned QuarterDenomMax   = DenomMax/4;
  static inline constexpr unsigned ThreeFourthDenomMax   = QuarterDenomMax*3;
  static inline constexpr unsigned TwiceDenomMax   = DenomMax*2;

  //TruncMultAsInt and HalfTruncMult in base file

  struct DLL_API RFOpResult : public RFOpResultBase<RestrictedFloat, OverflowType, Policy>{};

public:

  //BinFrac is in base file

  // Map RF -> (num / 2^shift)
  // Fixed slope formula overload to_binfrac(needs EnabledSmoothScale formula version)
  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  static inline BinFrac to_binfrac(unsigned e, unsigned s) {
    // DenomMax = 2^SIG_BITS
    const u128 D2 = u128{1} << (SIG_BITS + 1);    // 2*DenomMax
    u128 num = (u128{2} * (u128{1} << SIG_BITS)) - s; // 2*DenomMax - signif
    u32 shift = (e == 0) ? (SIG_BITS + 1) : (SIG_BITS + e + 1);
    if (e == 0 && s == 0) { num = 0; shift = 1; }   // sentinel zero: 0/2^1
    return { num, shift };
  }

  // Normalize (num / 2^shift) back to RF tail, extracting unitDelta (integer part)
  static inline RFOpResult
  from_binfrac_with_unit(u128 num, u32 shift) {
    RFOpResult out{};
    // Integer carry
    const u128 one = 1;
    u128 intPart = (shift < 128) ? (num >> shift) : u128{0};
    out.unitDelta = static_cast<OverflowType>(intPart);
    u128 fracNum  = (shift < 128) ? (num & ((one << shift) - 1)) : num; // all fractional if shift >= 128

    if (fracNum == 0) {
    out.tail = RestrictedFloat(); // zero
    return out;
    }

    // We want fracNum / 2^shift mapped to:
    //  - zero band: num' / 2^(SIG_BITS+1)  (exp=0)
    //  - general:   num' / 2^(SIG_BITS+e+1), with 1 <= num' <= 2^(SIG_BITS+1)
    // Try to compress numerator <= 2^(SIG_BITS+1) by increasing e as needed.

    const u128 LIMIT = u128{1} << (SIG_BITS + 1); // 2*DenomMax
    u32 baseShift = SIG_BITS + 1;         // zero-band denominator shift
    if (shift <= baseShift) {
    // Expand numerator so denominator becomes baseShift
    fracNum <<= (baseShift - shift);
    shift = baseShift;
    } else {
    // Reduce numerator by shifting right until it fits, tracking how many we consumed as exp increments
    u32 extra = shift - baseShift; // this equals (e+something)
    // Shift right as little as possible so fracNum' <= LIMIT
    u32 k = 0;
    if (fracNum > LIMIT) {
      // k = ceil_log2(fracNum / LIMIT)
      u128 tmp = fracNum / LIMIT;  // >=1
      while (tmp) { tmp >>= 1; ++k; }
      if (k > 0) --k; // adjust: we want smallest k s.t. (fracNum >> k) <= LIMIT
      while ((fracNum >> k) > LIMIT) ++k;
      fracNum >>= k;
      extra -= (extra >= k) ? k : extra; // keep extra non-negative
    }
    shift = baseShift + extra; // final denominator shift
    }

    // Now denominator is 2^(SIG_BITS + 1 + e), with e = shift - (SIG_BITS+1)
    u32 e = (shift > baseShift) ? (shift - baseShift) : 0;

    // Map to RF fields:
    // signif = 2*DenomMax - num'
    u128 numPrime = fracNum;
    if (numPrime > LIMIT) numPrime = LIMIT; // clamp (rare)
    u128 s128 = LIMIT - numPrime; // 0..(2*DenomMax - 1) ideally
    // Clamp significand to SIG_MAX range by folding the top bit:
    // If s128 >= 2*DenomMax, push a unit to intPart
    if (s128 >= LIMIT) {
    // This would mean numPrime==0; treat as integer carry
    out.unitDelta += OverflowType{1};
    out.tail = RestrictedFloat(); // zero tail
    return out;
    }
    unsigned s = static_cast<unsigned>(s128); // fits in [0,2*DenomMax)
    // Here, s must be <= SIG_MAX for stored encoding (25 bits). If s >= 2^SIG_BITS, we reflect:
    // Store only the lower SIG_BITS; excess top bit translates into the zero-band's MSB,
    // but your encoding reserves only SIG_BITS; so clamp safely:
    if (s > SIG_MAX) s = SIG_MAX;

    // e==0 uses zero band. e>0 uses general formula.
    unsigned storeExp = (e == 0) ? 0u : e - 1; // careful: denominator shift is SIG_BITS + (e), but your general band formula uses (exp+1)
    // Reconcile: shift = SIG_BITS + (exp+1) => e == exp+1 => exp = e-1
    out.tail = RestrictedFloat(storeExp, s);
    return out;
  }
  
  std::string ToFormulaFormat()
  {
    if (isZero())
    return "0";
    // Outputs string in "1.0 - (signif / TwiceDenomMax)" format (Result of above .5 to keep slope exact)
    else if (exp() == 0) {
    std::string outputStr = "1 - ";
    outputStr += VariableConversionFunctions::UIntToStringConversion(signif());
    outputStr += "/";
    outputStr += VariableConversionFunctions::UIntToStringConversion(TwiceDenomMax);
    return outputStr;
    }
    else if (signif() == 0) {
    unsigned int ExpTotal = (1u << exp());
    std::string outputStr = "1/";
    outputStr += VariableConversionFunctions::UIntToStringConversion(ExpTotal);
    return outputStr;
    }
    // Outputs string in (2 * DenomMax - SignifNum) / (DenomMax * (1 << (Exp + 1))) format
    else {
    unsigned int ExpTotal = (1u << (exp() + 1));
    std::string outputStr = "(";
    outputStr += VariableConversionFunctions::UIntToStringConversion(2u * DenomMax);
    outputStr += " - ";
    outputStr += VariableConversionFunctions::UIntToStringConversion(signif());
    outputStr += ")/";
    ExpTotal *= DenomMax;
    outputStr += VariableConversionFunctions::UIntToStringConversion(ExpTotal);
    return outputStr;
    }
  }

  // Pow10 tables are inside base file(following also in base file)
  //POW10_*
  //std::string to_dec_string
  //ScaleParams
  //mul_fits_bits

    //Might need SmoothScale version
    template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  	static inline ScaleParams make_scale_params(u32 signifNum, u32 fracBits, u32 exp) noexcept {
      u64 denomMax = u64{1} << fracBits;
      u64 num      = (denomMax << 1) - signifNum;
      u32 shift    = fracBits + (exp + 1); // when exp==0, this is fracBits+1, matching your zero-band
      return { num, shift };
  	}

    template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
    static inline std::string ZeroBandToString(uint32_t signifNum,
                         uint32_t fracBits,
                         uint32_t P) {
    u64 denomMax = u64{1} << fracBits;
    u64 num    = (denomMax << 1) - signifNum; // (2 * DenomMax) - signif
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

  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
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
  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
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
	
  /// <summary>
  /// Converts to string(Outputs string in digit display format).
  /// </summary>
  /// <returns>std.string</returns>
  template<typename = std::enable_if_t<Policy::EnabledSmoothScale>>
  std::string ToString(size_t maxDigits = 10) const {
    auto e = exp();
    auto s = signif();
    // Case: Exp=0, Signif=0 -> exact zero
    if (e == 0 && signif() == 0)
      return "0";

    //Code needs updated for smooth band
  }

  // sign-aware dispatch in MixedDec (Exp,SignifNum in comments)
	//  All Op prefix are designed to modify original object
	//  By prefix commands instead output a copy
	
  //All results with RestrictedFloat x RestrictedFloat division will overflow

  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  RFOpResult<RestrictedFloat,OverflowType,Policy>
	static DivideByCatchAll(const auto &e, const auto &s, const RestrictedFloat &x) const {
		RFOpResult out{};
		if (isZero()) { out.tail = RestrictedFloat(); return out; }

		auto R = to_binfrac(x.exp(), x.signif());
		if (R.num == 0) {
			// division by zero: saturate tail? here we set sticky + overflow_to_whole
			out.tail = RestrictedFloat();
			out.status.sticky = 1; out.status.overflow_to_whole = 1;
			return out;
    }

		auto L = to_binfrac(e, s);
		// (L / R) = (L.num << K) / R.num  over 2^(L.shift - R.shift), choose K to keep precision
		// Choose K big enough to preserve tail but within u128. Here: K = 64 (tunable)
		const u32 K = 64;
		u128 numScaled = (L.num << K);
		u128 q = numScaled / R.num;      // integer quotient
		u32  denonShift = (L.shift + 0) - R.shift + K; // resulting denominator shift

		out = from_binfrac_with_unit(q, denonShift);
		// Division can easily overflow to whole
		out.status.overflow_to_whole = (out.unitDelta != 0);
		// Inexact if remainder nonzero
		out.status.inexact = ((numScaled % R.num) != 0);
		out.status.sticky |= out.status.inexact;
		return out;
	}

  template<typename = std::enable_if_t<Policy::EnabledSmoothScale>>
  RFOpResult<RestrictedFloat,OverflowType,Policy>
	static DivideByCatchAll(const auto &e, const auto &s, const RestrictedFloat &x) const {
	}

  RFOpResult DivideBy(const RestrictedFloat &x) const {
    auto e = exp();
    auto s = signif();
		auto xe = x.exp();
		auto xs = x.signif();
		return DivideByCatchAll(e, s, x);
  }

  RFOpResult DivOp(const RestrictedFloat &x) const {
	  RFOpResult<RestrictedFloat,OverflowType,Policy> out = DivideBy(x);
		raw_ = out.tail;
	}

  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  RFOpResult static MultipleByCatchAll(const auto &e, const auto &s, const RestrictedFloat &x) const {
		RFOpResult<RestrictedFloat,OverflowType,Policy> out{};
		if (isZero() || x.isZero()) { out.tail = RestrictedFloat(); return out; }

		auto L = to_binfrac(e, s);
		auto R = to_binfrac(x.exp(), x.signif());
		// (l_num/2^l_s) * (r_num/2^r_s) = (l_num*r_num) / 2^(l_s + r_s)
		u128 prod = L.num * R.num;
		u32  denonShift  = L.shift + R.shift;

		out = from_binfrac_with_unit(prod, denonShift);
		// If product >= 1, unitDelta may be > 0
		out.status.overflow_to_whole = (out.unitDelta != 0);
		return out;
	}
	
  template<typename = std::enable_if_t<Policy::EnabledSmoothScale>>
  RFOpResult static MultipleByCatchAll(const auto &e, const auto &e, const RestrictedFloat &x) const {
	}

  RFOpResult MultipleBy(const RestrictedFloat &x) const {
    auto e = exp();
    auto s = signif();
		auto xe = x.exp();
		auto xs = x.signif();
		return MultipleByCatchAll(e, s, x);
  }

  RFOpResult MultOp(const RestrictedFloat &x) const {
	  RFOpResult out = DivideBy(x);
		raw_ = out.tail;
	}

  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  RFOpResult SameSignAddByCatchAll(const auto &e, const auto &s, const RestrictedFloat &x) const {
		RFOpResult out{};
		// Quick exits
		if (isZero()) { out.tail = x; return out; }
		if (x.isZero()) { out.tail = *this; return out; }

		// Align denominators
		auto L = to_binfrac(exp(), signif());
		auto R = to_binfrac(x.exp(), x.signif());
		u32 denonShift = std::max(L.shift, R.shift);
		u128 ln = L.num << (denonShift - L.shift);
		u128 rn = R.num << (denonShift - R.shift);
		u128 sum = ln + rn;

		out = from_binfrac_with_unit(sum, denonShift);
		// Sticky/inexact flags here only if you add rounding (currently exact)
		return out;
	}

  template<typename = std::enable_if_t<!Policy::EnabledSmoothScale>>
  RFOpResult SameSignSubByCatchAll(const auto &e, const auto &s, const RestrictedFloat &x) const {
		RFOpResult out{};
		if (isZero()) { out.tail = RestrictedFloat(); out.status.underflow_to_zero = 1; return out; }
		if (x.isZero()) { out.tail = *this; return out; }

		auto L = to_binfrac(e, s);
		auto R = to_binfrac(x.exp(), x.signif());
		u32 denonShift = std::max(L.shift, R.shift);
		u128 ln = L.num << (denonShift - L.shift);
		u128 rn = R.num << (denonShift - R.shift);

		if (ln <= rn) {
			out.tail = RestrictedFloat();
			out.status.underflow_to_zero = (ln < rn) ? 1u : 0u; // exact zero if equal
			return out;
		}
		u128 diff = ln - rn;
		out = from_binfrac_with_unit(diff, denonShift);
		return out;
	}

  template<typename = std::enable_if_t<Policy::EnabledSmoothScale>>
  RFOpResult SameSignAddByCatchAll(const RestrictedFloat &x) const {

  }

  template<typename = std::enable_if_t<Policy::EnabledSmoothScale>>
  RFOpResult SameSignSubByCatchAll(const RestrictedFloat &x) const {

  }

	// returns (integer_carry, result_tail)
	std::pair<uint32_t,LinearVal>
	addLinearInverted(const LinearVal& A, const LinearVal& B) {
			constexpr uint32_t DenomMax = 1u << fracBits;
			constexpr uint64_t FullDen  = uint64_t(2) * DenomMax;

			// 1) trivial zero‐tail
			//    zero‐tail means RawTail == 0 => (2*DenomMax - sig)==0 => sig==2*DenomMax
			if (A.sig == FullDen && A.exp == 0) return {0u, B};
			if (B.sig == FullDen && B.exp == 0) return {0u, A};

			// 2) align to max exponent
			uint32_t eMax   = std::max(A.exp, B.exp);
			uint32_t dA     = eMax - A.exp;
			uint32_t dB     = eMax - B.exp;

			// 3) recover inverted‐order numerators and shift down
			uint64_t numA = (FullDen - A.sig) >> dA;
			uint64_t numB = (FullDen - B.sig) >> dB;

			// 4) add, detect integer carry, subtract fullDen for remainder
			uint64_t sum   = numA + numB;
			uint32_t ic    = sum >= FullDen ? 1u : 0u;
			if (ic) sum -= FullDen;

			// 5) repack inverted‐order tail
			LinearVal R;
			R.exp = eMax;
			R.sig = uint32_t(FullDen - sum);

			return {ic, R};
	}

	// returns { borrow (0 or 1), resultTail }
	// caller can compute integer-part: intA - intB - borrow
	std::pair<uint32_t,LinearVal>
	subLinearInverted(const LinearVal& A, const LinearVal& B) {
			constexpr uint32_t DenomMax = 1u << fracBits;
			constexpr uint64_t FullDen  = uint64_t(2) * DenomMax;

			// 1) trivial B==0 => no borrow, result is A
			if (B.sig == FullDen && B.exp == 0) return {0u, A};

			// 2) align to max exponent
			uint32_t eMax = std::max(A.exp, B.exp);
			uint32_t dA   = eMax - A.exp;
			uint32_t dB   = eMax - B.exp;

			// 3) recover inverted-order numerators and shift down
			uint64_t numA = (FullDen - A.sig) >> dA;
			uint64_t numB = (FullDen - B.sig) >> dB;

			// 4) subtract, detect borrow, and normalize remainder
			bool     borrow = numA < numB;
			uint64_t diff   = numA - numB;
			if (borrow) diff += FullDen;    // borrow one “whole” tail

			// 5) repack inverted-order tail
			LinearVal R;
			R.exp = eMax;
			R.sig = uint32_t(FullDen - diff);

			return { borrow ? 1u : 0u, R };
	}


	// returns { borrow (0 or 1), resultTail }
	// integer-part borrow = how many whole units to subtract from the int-difference
	std::pair<uint32_t,BinFrac>
	subSmooth(const BinFrac& A, const BinFrac& B) {
			// 1) trivial B==0
			if (B.num == 0 && B.shift == 0) return {0u, A};

			// 2) align to max shift
			uint32_t sMax = std::max(A.shift, B.shift);
			uint64_t a    = A.num >> (A.shift - sMax);
			uint64_t b    = B.num >> (B.shift - sMax);

			// 3) subtract, detect borrow, normalize
			bool     borrow = a < b;
			uint64_t diff   = a - b;
			if (borrow) diff += (uint64_t(1) << sMax);

			// 4) repack
			BinFrac R;
			R.shift = sMax;
			R.num   = diff;  // already < 2^sMax
			return { borrow ? 1u : 0u, R };
	}

  RFOpResult SameSignAddBy(const RestrictedFloat &x) const {
    auto e = exp();
    auto s = signif();
		auto xe = x.exp();
		auto xs = x.signif();
		
		return SameSignAddByCatchAll(e, s, x);
  }

  //Both sides are either negative or positive
  RFOpResult SameSignSubBy(const RestrictedFloat &x) const {
    auto e = exp();
    auto s = signif();
		auto xe = x.exp();
		auto xs = x.signif();
		return SameSignSubByCatchAll(e, s, x);
  }

  RFOpResult SameSignAddOp(const RestrictedFloat &x) const {
	  RFOpResult out = SameSignAddBy(x);
		raw_ = out.tail;
	}
	
  RFOpResult SameSignSubOp(const RestrictedFloat &x) const {
	  RFOpResult out = SameSignSubBy(x);
		raw_ = out.tail;
	}

  //Left side is positive and right side is negative
  RFOpResult PositiveSignAddBy(const RestrictedFloat &x) const {

  }

  //Left side is negative and right side is positive
  RFOpResult NegativeSignAddBy(const RestrictedFloat &x) const {

  }

  //Left side is positive and right side is negative
  RFOpResult PositiveSignSubBy(const RestrictedFloat &x) const {

  }

  //Left side is negative and right side is positive
  RFOpResult NegativeSignSubBy(const RestrictedFloat &x) const {

  }

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
  
  std::strong_ordering operator<=>(const RestrictedFloat& that) const
  {
  if (isZero()) {
    if (that.isZero()) return 0 <=> 0;
    else return 0 <=> 1;
  }
  if (that.isZero()) return 1 <=> 0;

  auto L = to_binfrac(exp(), signif());
  auto R = to_binfrac(that.exp(), that.signif());
  // Align and compare
  u32 s = std::max(L.shift, R.shift);
  u128 ln = L.num << (s - L.shift);
  u128 rn = R.num << (s - R.shift);
  if (ln < rn) return std::strong_ordering::less;
  if (ln > rn) return std::strong_ordering::greater;
  return std::strong_ordering::equivalent;
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
      if (v == 0 || digits == 0) { raw_ = ZeroRaw; return; }

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
      raw_ = (StoreT(0) << SIG_BITS) | StoreT(s128 & SIG_MASK);
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
        // Too small even at EXP_MAX -> underflow to zero
        raw_ = ZeroRaw;
        return;
      }
      // s = 2D - target
      u128 s128 = twoD - target;
      if (s128 > twoD - 1) s128 = twoD - 1; // clamp
      raw_ = (StoreT(e) << SIG_BITS) | StoreT( static_cast<StoreT>(s128) & SIG_MASK );
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

};

  #pragma region ValueDefine Source

  RestrictedFloat RestrictedFloat::Zero = ZeroValue();

  #pragma endregion ValueDefine Source

} // namespace BlazesRusCode
