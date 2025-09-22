// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <type_traits>   // std::is_same_v
#include <cstddef>     // size_t
#include "../MediumDec/MediumUDec.hpp"
#include "../PartialDec/PartialUDec.hpp"
#include "../PartialDec/ExpandedUDec.hpp"//Used to store large results and then divide
#include <boost/rational.hpp>

namespace BlazesRusCode {

template<typename DH, DH Limit>
struct UnderOnePureGenerator {
  static constexpr auto generate() {
    std::array<DH, 64> table{};
    DH val = Limit / 2;
    size_t count = 0;
    while (val != 0) {
      table[count++] = val;
      if (val % 2 != 0) break; // next halving would need DecimalHalf
      val /= 2;
    }
    return std::pair{table, count};
  }

  inline static constexpr auto result = generate();
  inline static constexpr auto& values = result.first;
  inline static constexpr size_t count = result.second;
};

template<class VariantName>
class FloatingConverterCore {
public:
  using DecimalHalfValueT = typename VariantName::DecimalHalfValueT;

  // Toggle: use the hand-tuned manual tables when DecimalHalfValueT is uint32_t
  inline static constexpr bool UseManualTables =
    std::is_same_v<DecimalHalfValueT, uint32_t>;

  // -------------------------
  // Tier 1 — Pure integer phase (manual default for uint32)
  // -------------------------
  inline static constexpr uint32_t DefaultUnderOnePureBreakpoints[] = {
    500'000'000, // 0.5       (exp: 1)
    250'000'000, // 0.25      (exp: 2)
    125'000'000, // 0.125     (exp: 3)
     62'500'000, // 0.0625    (exp: 4)
     31'250'000, // 0.03125     (exp: 5)
     15'625'000, // 0.015625    (exp: 6)
      7'812'500, // 0.0078125   (exp: 7)
      3'906'250, // 0.00390625  (exp: 8)
      1'953'125  // 0.001953125   (exp: 9) last pure integer IntHalf
  };

  // For non-uint32 DecimalHalfValueT, you can point this to a generated table
  // via VariantName or another traits object (same type and meaning).
  // By default we reuse the manual table.
  inline static constexpr auto& UnderOnePureBreakpoints =
  UseManualTables ? DefaultUnderOnePureBreakpoints : UnderOnePureGenerator<DH, Limit>::values;

  inline static constexpr size_t PureCount =
  UseManualTables ? std::size(DefaultUnderOnePureBreakpoints) : UnderOnePureGenerator<DH, Limit>::count;

  // Indexes (exponents) where phases begin. For uint32_t, these are manual.
  // For larger DecimalHalfValueT, override in VariantName (e.g., supply larger runs).
  inline static constexpr size_t PartialIndexStart =
    std::is_same_v<DecimalHalfValueT, uint32_t> ? 10 : UnderOnePureBreakpoints<DecimalHalfValueT, VariantName>::count;

  inline static constexpr size_t ExtremeIndexStart =
    std::is_same_v<DecimalHalfValueT, uint32_t> ? 19 : PartialIndexStart+9;

  inline static constexpr size_t ExtremeIndexEnd =
    std::is_same_v<DecimalHalfValueT, uint32_t> ? 30 : ExtremeIndexStart+11;

  // -------------------------
  // Tier 2 — Partial phase (MediumUDec)
  // -------------------------
  inline static constexpr MediumUDec UnderOnePartialBreakpoints[] = {
    MediumUDec(  976'562,     5),  // 0.0009765625    (exp: 10)
    MediumUDec(  488'281,    25),  // 0.00048828125     (exp: 11)
    MediumUDec(  244'140,   625),  // 0.000244140625    (exp: 12)
    MediumUDec(  122'070,   3'125),  // 0.0001220703125   (exp: 13)
    MediumUDec(   61'035,  15'625),  // 0.00006103515625  (exp: 14)
    MediumUDec(   30'517,  578'125),   // 0.000030517578125   (exp: 15)
    MediumUDec(   15'258, 7'890'625),  // 0.0000152587890625  (exp: 16)
    MediumUDec(  7'629, 39'453'125), // 0.00000762939453125 (exp: 17)
    MediumUDec(  3'814,697'265'625)  // 0.000003814697265625(exp: 18) Last before needing smaller representation
  };

  // -------------------------
  // Tier 3 — Extreme phase (TinyUDecV2 or PartialUDec)
  // -------------------------
  // Pick the element type at compile time
  using ExtremeBPType = std::conditional_t<
      VariantName::HasTinyUDec,
      TinyUDecType,   // e.g., TinyUDecV2
      PartialUDec
  >;

  inline static constexpr ExtremeBPType UnderOneExtremeBreakpoints[] = []{
		return std::array<ExtremeBPType, 12>{
			ExtremeBPType(3'814,697'265'625, 2),
			ExtremeBPType(3'814,697'265'625, 4),
			ExtremeBPType(3'814,697'265'625, 8),
			ExtremeBPType(3'814,697'265'625, 16),
			ExtremeBPType(3'814,697'265'625, 32),
			ExtremeBPType(3'814,697'265'625, 64),
			ExtremeBPType(3'814,697'265'625, 128),
			ExtremeBPType(3'814,697'265'625, 256),
			ExtremeBPType(3'814,697'265'625, 512),
			ExtremeBPType(3'814,697'265'625, 1024),
			ExtremeBPType(3'814,697'265'625, 2048),
			ExtremeBPType(3'814,697'265'625, 4096)
		};
  }();

  template<class RF=RestrictedFloat>
  constexpr RF to_restricted_float() {
    using DH    = typename VariantName::DecimalHalfValueT;
    using SignifT = typename RF::SignifT;
    constexpr auto MaxDenom = RF::DenomMax;
  
    if (IsNegative()) throw overflow_error{"RestrictedFloat negative overflow"};
    if (IsGreaterOrEqualToOne()) throw overflow_error{"RestrictedFloat positive overflow"};
    if(IsZero()) return RF::Zero;
  
    DH decPart = GetDecimalHalf();
    SignifT signifNum;
    // --- Phase 1: Pure integer breakpoints ---
    {
      DH prev_bp = VariantName::DecimalOverflow; // 1.0 in DH units
      for (size_t i = 0; i < PureCount; ++i) {
        DH curr_bp = UnderOnePureBreakpoints[i];
        if (decPart >= curr_bp) {
          boost::rational<DH> frac(prev_bp - decPart,
                       prev_bp - curr_bp);
          frac = boost::rational<DH>(frac.numerator(), frac.denominator()); // reduce
  
          signifNum = static_cast<SignifT>(
            (frac.numerator() * MaxDenom) / frac.denominator()
          );
          return RF(static_cast<uint16_t>(i), signifNum); // exp = i
        }
        prev_bp = curr_bp;
      }
    }
    //Separate compare blocks for transitions
    //(Representations of MediumDec==IntHalf*DecimalOverFlow+DecimalHalf)
    // --- Phase 2: Partial breakpoints(Transition stage) ---
    ExpandedUDec signifTemp;
    {
      auto prev_bp = UnderOnePureBreakpoints[PureCount - 1]; // last pure entry
      const auto& curr_bp = UnderOnePartialBreakpoints[0];
      if (decPart >= curr_bp.DecimalHalf) {
        auto remainderV = prev_bp - decPart;
        auto intervalV  = prev_bp - curr_bp;

        //First numerator half can result in:32'767'983'222'784 which is too big for MediumUDec
        signifTemp = remainderV * RF::MaxDenom;
        signifTemp /= intervalV;

        signifNum = static_cast<SignifT>(signifTemp);
        return RF(static_cast<uint16_t>(PureCount + i), signifNum);
      }
    }
    // --- Phase 2: Partial breakpoints ---
    {
      auto prev_bp = UnderOnePartialBreakpoints[0]; // first entry
      for (size_t i = 1; i < PartialCount; ++i) {
        const auto& curr_bp = UnderOnePartialBreakpoints[i];
        if (decPart >= curr_bp.DecimalHalf) {
          auto remainderV = prev_bp - decPart;
          auto intervalV  = prev_bp - curr_bp;
  
          signifTemp = remainderV * RF::MaxDenom;
          signifTemp /= intervalV;

          signifNum = static_cast<SignifT>(signifTemp);
          return RF(static_cast<uint16_t>(PureCount + i), signifNum);
        }
        prev_bp = curr_bp;
      }
    }
  
    // --- Phase 3: Extreme breakpoints(Transition stage) ---
    {
      ExtremeBPType prev_bp = UnderOnePartialBreakpoints[PartialCount - 1]; // last partial entry
      const auto& curr_bp = UnderOneExtremeBreakpoints[0];
      if (decPart >= curr_bp.DecimalHalf) {
        auto remainderV = prev_bp - decPart;
        auto intervalV  = prev_bp - curr_bp;
  
        signifTemp = remainderV * RF::MaxDenom;
        signifTemp /= intervalV;

        signifNum = static_cast<SignifT>(signifTemp);
        return RF(static_cast<uint16_t>(PureCount + PartialCount), signifNum);
      }
    }
    // --- Phase 3: Extreme breakpoints ---
    {
      ExtremeBPType prev_bp = UnderOneExtremeBreakpoints[0];
      size_t ExtremeLimit = ExtremeCount - 1;
      for (size_t i = 0; i < ExtremeLimit; ++i) {
        const auto& curr_bp = UnderOneExtremeBreakpoints[i];
        if (decPart >= curr_bp.DecimalHalf) {
          auto remainderV = prev_bp - decPart;
          auto intervalV  = prev_bp - curr_bp;
  
          signifTemp = remainderV * RF::MaxDenom;
          signifTemp /= intervalV;

          signifNum = static_cast<SignifT>(signifTemp);
          return RF(static_cast<uint16_t>(PureCount + PartialCount + i), signifNum);
        }
        prev_bp = curr_bp;
      }
      // Final interval: penultimate → last
      auto last_bp = bp_dec<DH>(UnderOneExtremeBreakpoints[ExtremeLimit]);   // 0.931'322'574'615'478'515625
      // At this point prevDecX is the penultimate: 1.862'645'149'230'957'03125
      auto remainderV = prev_bp - decPart;
      auto intervalV  = prev_bp - curr_bp;

      signifTemp = remainderV * RF::MaxDenom;
      signifTemp /= intervalV;

      signifNum = static_cast<SignifT>(signifTemp);
      return RF(static_cast<uint16_t>(PureCount + PartialCount + i), signifNum);
   }
  
    // -------------------------
    // Should never be reached since would be at 0.931'322'574'615'478'515625 DecimalHalf range at this point
    // -------------------------
    #if defined(DEBUG) || defined(_DEBUG)
      throw std::logic_error(
        "extract_exp_signif(): No breakpoint matched — table coverage incomplete or value out of range"
      );
    #elif defined(_MSC_VER)
       __assume(false);   // MSVC
    #else
      __builtin_unreachable(); // GCC/Clang
    #endif
  }

  template<typename BPType, typename DH, typename SignifT, auto MaxDenom>
  constexpr std::pair<int16_t, SignifT>
  extract_fraction_step(const BPType& prev_bp, const BPType& curr_bp,
  const VariantName& value, int16_t exp) {
      auto remainderV = prev_bp - value;
      auto intervalV  = prev_bp - curr_bp;
    
      boost::rational<DH> frac(
        remainderV.DecimalHalf(),
        intervalV.DecimalHalf()
      );
      frac = { frac.numerator(), frac.denominator() };
    
      SignifT signifNum = static_cast<SignifT>(
        (frac.numerator() * MaxDenom) / frac.denominator()
      );
    
     return { exp, signifNum };
  }
    
  template<typename SignifT, auto MaxDenom>
  constexpr std::pair<int16_t, SignifT>
  extract_exp_signif() const {
    using DH = DecimalHalfValueT;
  
    if (is_zero()) return {0, 0};
  
    auto intPart = GetIntHalf(); // unsigned magnitude
  
    // -------------------------
    // ≥ 1 path: bit-scan exponent
    // -------------------------
    if (intPart > 0) {
      #if defined(__GNUC__) || defined(__clang__)
      uint16_t exp = std::numeric_limits<IntHalfValueT>::digits - 1
           - static_cast<uint16_t>(__builtin_clz(intPart));
      #elif defined(_MSC_VER)
      unsigned long idx;
      _BitScanReverse(&idx, intPart);
      uint16_t exp = static_cast<uint16_t>(idx);
      #else
      uint16_t exp = 0;
      auto tmp = intPart;
      while (tmp >>= 1) ++exp;
      #endif
    
      IntHalfValueT lowerInt = IntHalfValueT(1) << exp;
      IntHalfValueT upperInt = (exp + 1 < std::numeric_limits<IntHalfValueT>::digits)
             ? (IntHalfValueT(1) << (exp + 1))
             : std::numeric_limits<IntHalfValueT>::max();
    
      auto remainderV = *this - lowerInt; // integer subtraction supported
      auto intervalV  = upperInt - lowerInt; // pure integer
    
      boost::rational<DH> frac(
        remainderV.DecimalHalf(),
        static_cast<DH>(intervalV)
      );
      frac = { frac.numerator(), frac.denominator() };
    
      SignifT signifNum = static_cast<SignifT>(
        (frac.numerator() * MaxDenom) / frac.denominator()
      );
    
      return { static_cast<int16_t>(exp), signifNum };
    }
  
    DH decPart = GetDecimalHalf();
    SignifT signifNum;
    int16_t exp;
    // --- Phase 1: Pure integer breakpoints ---
    {
      DH prev_bp = VariantName::DecimalOverflow; // 1.0 in DH units
      for (size_t i = 0; i < PureCount; ++i) {
        DH curr_bp = UnderOnePureBreakpoints[i];
        if (decPart >= curr_bp) {
          boost::rational<DH> frac(prev_bp - decPart,
                       prev_bp - curr_bp);
          frac = boost::rational<DH>(frac.numerator(), frac.denominator()); // reduce
  
          signifNum = static_cast<SignifT>(
            (frac.numerator() * MaxDenom) / frac.denominator()
          );
          exp = static_cast<uint16_t>(i)*-1;
          return { exp, signifNum };
        }
        prev_bp = curr_bp;
      }
    }
    //Separate compare blocks for transitions
    //(Representations of MediumDec==IntHalf*DecimalOverFlow+DecimalHalf)
    // --- Phase 2: Partial breakpoints(Transition stage) ---
    ExpandedUDec signifTemp;
    {
      auto prev_bp = UnderOnePureBreakpoints[PureCount - 1]; // last pure entry
      const auto& curr_bp = UnderOnePartialBreakpoints[0];
      if (decPart >= curr_bp.DecimalHalf) {
        auto remainderV = prev_bp - decPart;
        auto intervalV  = prev_bp - curr_bp;

        //First numerator half can result in:32'767'983'222'784 which is too big for MediumUDec
        signifTemp = remainderV * RF::MaxDenom;
        signifTemp /= intervalV;

        signifNum = static_cast<SignifT>(signifTemp);
        exp = static_cast<uint16_t>(PureCount)*-1;
        return { exp, signifNum };
      }
    }
    // --- Phase 2: Partial breakpoints ---
    {
      auto prev_bp = UnderOnePartialBreakpoints[0]; // first entry
      for (size_t i = 1; i < PartialCount; ++i) {
        const auto& curr_bp = UnderOnePartialBreakpoints[i];
        if (decPart >= curr_bp.DecimalHalf) {
          auto remainderV = prev_bp - decPart;
          auto intervalV  = prev_bp - curr_bp;
  
          signifTemp = remainderV * RF::MaxDenom;
          signifTemp /= intervalV;

          signifNum = static_cast<SignifT>(signifTemp);
          exp = static_cast<uint16_t>(PureCount + i)*-1;
          return { exp, signifNum };
        }
        prev_bp = curr_bp;
      }
    }
  
    // --- Phase 3: Extreme breakpoints(Transition stage) ---
    {
      ExtremeBPType prev_bp = UnderOnePartialBreakpoints[PartialCount - 1]; // last partial entry
      const auto& curr_bp = UnderOneExtremeBreakpoints[0];
      if (decPart >= curr_bp.DecimalHalf) {
        auto remainderV = prev_bp - decPart;
        auto intervalV  = prev_bp - curr_bp;
  
        signifTemp = remainderV * RF::MaxDenom;
        signifTemp /= intervalV;

        signifNum = static_cast<SignifT>(signifTemp);
        exp = static_cast<uint16_t>(PureCount + PartialCount)*-1;
        return { exp, signifNum };
      }
    }
    // --- Phase 3: Extreme breakpoints ---
    {
      ExtremeBPType prev_bp = UnderOneExtremeBreakpoints[0];
      size_t ExtremeLimit = ExtremeCount - 1;
      for (size_t i = 0; i < ExtremeLimit; ++i) {
        const auto& curr_bp = UnderOneExtremeBreakpoints[i];
        if (decPart >= curr_bp.DecimalHalf) {
          auto remainderV = prev_bp - decPart;
          auto intervalV  = prev_bp - curr_bp;
  
          signifTemp = remainderV * RF::MaxDenom;
          signifTemp /= intervalV;

          signifNum = static_cast<SignifT>(signifTemp);
          exp = static_cast<uint16_t>(PureCount + PartialCount + i)*-1;
          return { exp, signifNum };
        }
        prev_bp = curr_bp;
      }
      // Final interval: penultimate → last
      auto last_bp = bp_dec<DH>(UnderOneExtremeBreakpoints[ExtremeLimit]);   // 0.931'322'574'615'478'515625
      // At this point prevDecX is the penultimate: 1.862'645'149'230'957'03125
      auto remainderV = prev_bp - decPart;
      auto intervalV  = prev_bp - curr_bp;

      signifTemp = remainderV * RF::MaxDenom;
      signifTemp /= intervalV;

      SignifT signifNum = static_cast<SignifT>(signifTemp);
      exp = static_cast<uint16_t>(PureCount + PartialCount + ExtremeLimit);
      return { exp, signifNum };
    }
    
    // -------------------------
    // Should never be reached since would be at 0.931'322'574'615'478'515625 DecimalHalf range at this point
    // -------------------------
    #if defined(DEBUG) || defined(_DEBUG)
      throw std::logic_error(
        "extract_exp_signif(): No breakpoint matched — table coverage incomplete or value out of range"
      );
    #elif defined(_MSC_VER)
       __assume(false);   // MSVC
    #else
      __builtin_unreachable(); // GCC/Clang
    #endif
  }

	float to_float() {
			using SignifT = uint32_t;
			constexpr auto MaxDenom = 1u << 24; // 24‑bit mantissa

			auto [exp, signif] = extract_exp_signif<SignifT, MaxDenom>();

			float mant = static_cast<float>(signif) / static_cast<float>(MaxDenom);
			float value = std::ldexp(mant, exp);
			return IsNegative() ? -value : value;
	}

	double to_double() {
			using SignifT = uint64_t;
			constexpr auto MaxDenom = 1ull << 53; // 53‑bit mantissa

			auto [exp, signif] = extract_exp_signif<SignifT, MaxDenom>();

			double mant = static_cast<double>(signif) / static_cast<double>(MaxDenom);
			double value = std::ldexp(mant, exp);
			return IsNegative() ? -value : value;
	}

	long double toLongDouble() const {
	#if LDBL_MANT_DIG == 64
			using SignifT = uint64_t;
			constexpr auto MaxDenom = 1ull << 64;
	#elif LDBL_MANT_DIG == 113
			using SignifT = unsigned __int128;
			constexpr auto MaxDenom = static_cast<SignifT>(1) << 113;
	#else
			using SignifT = uint64_t;
			constexpr auto MaxDenom = 1ull << 53;
	#endif

			auto [exp, signif] = extract_exp_signif<SignifT, MaxDenom>();

			long double mant = static_cast<long double>(signif) / static_cast<long double>(MaxDenom);
			long double value = std::ldexpl(mant, exp);
			return IsNegative() ? -value : value;
	}

};

} // namespace BlazesRusCode
