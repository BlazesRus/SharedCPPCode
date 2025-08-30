// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// Code adjusted with help of Microsoft Copilot
// ***********************************************************************
#pragma once
#include <cstdint>
#include <array>
#include <limits>
#include <bit>      // C++20 std::bit_width
#include <climits> // for CHAR_BIT
#include <string>
#include <type_traits> //for std::enable_if_t etc
#if !defined(__SIZEOF_INT128__)
  #include <boost/multiprecision/cpp_int.hpp>
#endif
#include <concepts> //C++20 concept

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

  // carry & status back to MixedDec
  struct RFStatus {
    unsigned overflow_to_whole : 1;
    unsigned underflow_to_zero : 1;
    unsigned inexact           : 1;
    unsigned sticky            : 1;
    unsigned reserved          : 4;
  };
  static_assert(sizeof(RFStatus) == 1, "Pack RFStatus to 1 byte");

  //Default Sentinal Policy for RestrictedFloat
  struct RFDefaultPolicy {
    // Compile-time knobs
		//Defaulting using reverse linear formula of "1.0 - (signif / TwiceDenomMax" for zero band

    //Toggle smooth scale to switch formula
    //from fixed point interpretation
    //to smooth scale interpretation(costs a little more to convert to real)
    /*
    if (n:0,signif:0){ RawResult = 0;}
		else if (n:0,signfi>0), then RawResult = "1.0 - (signif / TwiceDenomMax"
		else if(EnableLUTMode)//Costs more 4Kb ROM footprint but is faster 
		{
		}
    else if(EnabledSmoothScale){
      RawResult = 2^(−(exp + signif/DenomMax))
    } else
    {
      else if (n>0,signfi>0), then RawResult = ~"(2 * DenomMax - SignifNum) / (DenomMax * (1 << (exp + 1)))"//Not as accurate as some smooth band calculation
    }
    */
    //representation (RawResult is before TailScaleFactor is applied for actual real number representation)
    //Accuracy lost if from truncation not approximation in this implimentation.
    //exp is n after NScaleFactor is applied if not 1
    static constexpr bool EnabledLUTMode = false;

    //Maximum normal exp band value stored for n (aliased to exp)
    static constexpr unsigned MaxExp = 127;
  
    using WideT       = u128;
    using OverflowType= u64;
    using StoreT      = u32;

    static constexpr unsigned expBits = 7;

    static constexpr unsigned fracBits = 25;

    //Not used yet(for possible n scaling later)
    static constexpr unsigned NScaleFactor = 1;

    // Outcome construction
    static inline RFStatus ok(bool inexact=false, bool sticky=false) noexcept {
      return RFStatus{0,0, static_cast<std::uint8_t>(inexact), static_cast<std::uint8_t>(sticky), 0};
    }

    // Canonicalized sentinel handling (single pass)
    template<class TailCore>
    static inline void on_overflow_to_whole(TailCore& tail, RFStatus& st) noexcept {
      st.overflow_to_whole = 1;
      // clamp tail to max-in-lattice below 1
    }
    template<class TailCore>
    static inline void on_underflow_to_zero(TailCore& tail, RFStatus& st) noexcept {
      st.underflow_to_zero = 1;
      // set zero band
    }

    static_assert(
      expBits + fracBits + 1 
        <= CHAR_BIT * sizeof(StoreT),
      "RFDefaultPolicy: expBits+fracBits+1 must fit in StoreT"
    );
  };

  template<typename P>
  concept RFPolicyVariant =
    // Type aliases
    requires { typename P::WideT; typename P::OverflowType; typename P::StoreT; } &&
  
    // True constexpr flags
    //(std::same_as<decltype(P::EnabledSmoothScale), const bool> &&
    // std::same_as<decltype(P::EnabledSubnormals),   const bool>) &&
  
    // Integral constants
    (std::same_as<decltype(P::MaxExp),      const unsigned> &&
     std::same_as<decltype(P::expBits),     const unsigned> &&
     std::same_as<decltype(P::fracBits),    const unsigned> &&
     std::same_as<decltype(P::NScaleFactor),const unsigned>) &&
  
    // Sentinel constructors
    requires { { P::ok(false, false) } -> std::same_as<RFStatus>; } &&
  
    // Overflow/underflow hooks
    requires(P p, typename P::WideT& w, RFStatus& s) {
      { P::on_overflow_to_whole(w, s) }   -> std::same_as<void>;
      { P::on_underflow_to_zero(w, s) }   -> std::same_as<void>;
    };

  template<class TailCore, class CarryT = std::uint64_t, class Policy = RFDefaultPolicy>
  struct RFOpResultBase : Policy {
    // 1) Enforce API compliance
    static_assert(RFPolicyVariant<Policy>,
      "Policy must satisfy RFPolicyVariant");
  
    // 2) Enforce bit-width packing for ANY policy
    static_assert(
      Policy::expBits + Policy::fracBits + 1
        <= CHAR_BIT * sizeof(typename Policy::StoreT),
      "Policy: expBits+fracBits+1 must fit in StoreT"
    );

    TailCore  tail;      // normalized [0,1) tail(Not used for op prefix)
    // raw integer bits part generated by mul/div or tail rounding carry
    // that is then dealt with by owner class that holds RestrictedFloat
    // which might need to be scaled (especially for division)
    CarryT    carry;     
    RFStatus  status{};
  };

  /// <summary>
  /// Base file for RestrictedFloat with non-template specific parts
  /// </summary>
  class DLL_API RestrictedFloatBase {
  protected:
    // your 10^19 multiplier
    static inline constexpr u128 TruncMultAsInt = 10000000000000000000ull;
    static inline constexpr u128 HalfTruncMult = 5000000000000000000ull;

  public:
  
  		struct BinFrac {
  			u128 num;     // numerator (>=0)
  			u32  shift;   // value = num / 2^shift
  		};
  
			struct LinearVal {
				uint32_t exp;     // exponent
				uint32_t sig;     // inverted‐order significand in [0, 2*DenomMax]
		  };
	
  		// Pow10 tables (extend if you need more P)
    #if HAVE_NATIVE_UInt128
      static constexpr std::array<u128, 39> POW10_U128 = []{
        std::array<u128,39> a{}; a[0]=1;
        for (size_t i=1;i<a.size();++i) a[i]=a[i-1]*10u;
        return a;
      }();
  
  		// bit-length of 10^P for fast safety checks (floor(log2(10^P)) + 1)
  		// Up to P=38 (fits in u128). Extend if you extend POW10_U128.
      static constexpr std::array<unsigned,39> POW10_BITLEN = []{
        std::array<unsigned,39> b{};
        for (size_t i=0;i<b.size();++i) {
          u128 v = POW10_U128[i]; unsigned bits=0;
          while (v) { v >>= 1; ++bits; }
          b[i] = bits ? bits : 1;
        }
        return b;
      }();
    #else
      static const std::array<u128, 39> POW10_U128 = []{
        std::array<u128,39> a{}; a[0]=1;
        for (size_t i=1;i<a.size();++i) a[i]=a[i-1]*u128{10};
        return a;
      }();
  
      static const std::array<unsigned,39> POW10_BITLEN = []{
        std::array<unsigned,39> b{};
        for (size_t i=0;i<b.size();++i) {
          u128 v = POW10_U128[i]; unsigned bits=0;
          while (v) { v >>= 1; ++bits; }
          b[i] = bits ? bits : 1;
        }
        return b;
      }();
    #endif
  		static constexpr std::array<u32, 10> POW10_U32 {
  				1u, 10u, 100u, 1'000u, 10'000u, 100'000u, 1'000'000u, 10'000'000u, 100'000'000u, 1'000'000'000u
  		};
  		static constexpr std::array<u64, 20> POW10_U64 {
  				1ull, 10ull, 100ull, 1'000ull, 10'000ull, 100'000ull, 1'000'000ull, 10'000'000ull, 100'000'000ull, 1'000'000'000ull,
  				10'000'000'000ull, 100'000'000'000ull, 1'000'000'000'000ull, 10'000'000'000'000ull, 100'000'000'000'000ull,
  				1'000'000'000'000'000ull, 10'000'000'000'000'000ull, 100'000'000'000'000'000ull, 1'000'000'000'000'000'000ull, 10'000'000'000'000'000'000ull
  		};
  
protected:
    static inline constexpr u64 TailScaleFactor = 1'000'000'000ull;

public:

  		template <class UInt>
  		static inline std::string to_dec_string(UInt v) {
  				if constexpr (std::is_same_v<UInt, u64> || std::is_same_v<UInt, u32>) {
  						return std::to_string(static_cast<u64>(v));
  				} else { // u128
  						if (v == 0) return "0";
  						char buf[64];
  						int idx = 64;
  						while (v != 0) {
  								u128 q = v / 10u;
  								unsigned digit = static_cast<unsigned>(v - q * 10u);
  								buf[--idx] = char('0' + digit);
  								v = q;
  						}
  						return std::string(buf + idx, buf + 64);
  				}
  		}
  
     // ScaleParams packs everything needed to form the raw fractional tail in [0,1):
     //   • Linear (fixed-point) mode(default):
     //       raw = (2*DenomMax - signifNum)
     //             / (DenomMax * 2^(exp+1))
     //   • Smooth (log-scale) mode(EnabledSmoothScale:true):
     //       raw = 2^(−(exp + signifNum/DenomMax))
     //       (packed as num/2^shift via fixed-point precision)
  		struct ScaleParams {
        u64   num;    // Numerator for raw-tail:
                      //   – linear: 2*DenomMax - signifNum
                      //   – smooth: scaled 2^(−(exp + signifNum/DenomMax))

        u32   shift;  // Denominator exponent (2^shift):
                     //   – linear: fracBits + (exp + 1)
                     //   – smooth: fixed-point precision + exp
  		};
  
  		// Cheap pre-check: will num * 10^P fit in N bits? (no 128 ops used)
  		template <unsigned N>
  		static inline bool mul_fits_bits(u64 num, unsigned pow10Bitlen) noexcept {
  				// bitlen(num) + bitlen(10^P) - 1 <= N
  				unsigned nb = std::bit_width(num);
  				if (nb == 0) nb = 1;
  				return (nb + pow10Bitlen - 1) <= N;
  		}
  };

} // namespace BlazesRusCode
