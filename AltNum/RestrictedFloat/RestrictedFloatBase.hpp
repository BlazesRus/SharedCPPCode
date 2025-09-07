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


  //Default Sentinal Policy for RestrictedFloat
  struct RFDefaultPolicy {

    static constexpr bool EnabledLUTMode = false;

    //Maximum normal exp band value stored for n (aliased to exp)
    static constexpr unsigned MaxExp = 127;
  
    using WideT       = u128;
    using OverflowType= u64;
    using StoreT      = u32;

    static constexpr unsigned EXP_BITS = 7;

    static constexpr unsigned SIG_BITS = 25;

    //Not used yet(for possible n scaling later)
    static constexpr unsigned NScaleFactor = 1;
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

		// Cheap pre-check: will num * 10^P fit in N bits? (no 128 ops used)
		template <unsigned N>
		static inline bool mul_fits_bits(u64 num, unsigned pow10Bitlen) noexcept {
				// bitlen(num) + bitlen(10^P) - 1 <= N
				unsigned nb = std::bit_width(num);
				if (nb == 0) nb = 1;
				return (nb + pow10Bitlen - 1) <= N;
		}

  	// Q-bit fixed-point config
  	static constexpr unsigned Q = 56; // choose >= SIG_BITS + ~8 for precision headroom
  	using uq_t = unsigned __int128;   // for wide multiplies
  	using uq64 = uint64_t;
  
  	static constexpr uq64 Q_ONE  = uq64(1) << Q;
  	static constexpr uq64 Q_MASK = Q_ONE - 1;
  
  	// Integer Newton-Raphson sqrt for Q-fixed in [0, 1<<Q]
  	static inline uq64 sqrtQ(uq64 a) noexcept {
  			if (a == 0) return 0;
  			// Initial guess: scale down to 64-bit domain
  			int lz = std::countl_zero(a);
  			// Normalize a to ~top bit at 62..63
  			int s = (lz & ~1); // even shift to keep pairs for sqrt
  			uq64 n = a << s;
  			// Initial x roughly 1<<((Q + 64 - s)/2)
  			uq64 x = uq64(1) << ((Q + 62 - s) / 2);
  
  			// Newton iterations: x_{k+1} = (x + a/x)/2 in Q-fixed
  			// Division in Q: a/x means ( (__int128)a << Q ) / x
  			for (int it = 0; it < 6; ++it) {
  					uq_t num = (uq_t(n) << Q);
  					uq64 ax  = static_cast<uq64>(num / x);
  					uq64 nx  = (x + ax) >> 1;
  					if (nx == x) break;
  					x = nx;
  			}
  			return x;
  	}

    // r[j] = 2^(-2^-j) in Q-fixed, j = 0..SIG_BITS-1
    template<unsigned SIG_BITS>
    struct RTable {
        uq64 r[SIG_BITS];
    
        constexpr RTable() : r{} {}
    
        static RTable build() {
            RTable t;
            // r0 = 1/2
            t.r[0] = Q_ONE >> 1;
            for (unsigned j = 1; j < SIG_BITS; ++j)
                t.r[j] = sqrtQ(t.r[j-1]);
            return t;
        }
    
        // Compose 2^(-i/Den) where Den = 1<<SIG_BITS, i in [0, Den)
        uq64 pow2_neg_frac(unsigned i) const {
            uq_t acc = Q_ONE; // start at 1.0 in Q
            for (unsigned j = 0; j < SIG_BITS; ++j) {
                if (i & (1u << j)) {
                    acc = (acc * r[j]) >> Q;
                }
            }
            return static_cast<uq64>(acc);
        }
    
        // Greedy inverse: given y in [1/2,1) in Q, find f in [0,Den) s.t.
        // prod_j r[j]^{bit_j} <= y and as tight as possible (floor).
        unsigned frac_index_from_Q(uq64 y) const {
            uq_t prod = Q_ONE;
            unsigned f = 0;
            // Walk bits from MSB down for stable greedy
            for (int j = int(SIG_BITS) - 1; j >= 0; --j) {
                uq_t trial = (prod * r[j]) >> Q;
                if (trial <= y) {
                    f |= (1u << j);
                    prod = trial;
                }
            }
            return f;
        }
    };

  };

} // namespace BlazesRusCode
