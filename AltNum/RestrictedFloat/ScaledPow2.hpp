// ***********************************************************************
// Code created with help of Microsoft Copilot
// ***********************************************************************
#pragma once

namespace BlazesRusCode {
#if defined(__SIZEOF_INT128__)
  using uInt128 = unsigned __int128;   // native, fastest when available
  #define HAVE_NATIVE_UInt128 1
#else
  using uInt128 = boost::multiprecision::uint128_t;
  #define HAVE_NATIVE_UInt128 0
#endif

// Helpers
inline int clz64(uint64_t x) {
#if defined(_MSC_VER)
  unsigned long idx;
  return _BitScanReverse64(&idx, x) ? 63 - static_cast<int>(idx) : 64;
#else
  return x ? __builtin_clzll(x) : 64;
#endif
}

inline int flog2_u64(uint64_t x) { return 63 - clz64(x); }

struct ScaledPow2 {
  // Value = mantissa * 2^(-exp).
  // In 64-bit lane: mant64 valid, exp <= 63.
  // In 128-bit lane: mant128 valid.
  bool wide = false;
  uint64_t mant64 = 0;
  uInt128     mant128 = 0;
  uint32_t exp = 0;

  // Constructors
  static ScaledPow2 from_uint64(uint64_t m, uint32_t e = 0) {
    ScaledPow2 v;
    v.mant64 = m;
    v.exp = e;
    v.wide = false;
    v.normalize();
    return v;
  }

  // Query
  bool is_zero() const { return wide ? (mant128 == 0) : (mant64 == 0); }

  // Normalize lane based on exp and mantissa size
  void normalize() {
    if (!wide) {
      if (mant64 == 0) { exp = 0; return; }
      if (exp <= 63) return;
      // If fractional depth already beyond 2^-64, promote so future ops have room.
      promote();
    } else {
      if (mant128 == 0) { wide = false; mant64 = 0; exp = 0; return; }
      // Demote only if safe: exp <= 63 and upper 64 bits zero.
      if (exp <= 63) {
        uint64_t hi = static_cast<uint64_t>(mant128 >> 64);
        if (hi == 0) {
          mant64 = static_cast<uint64_t>(mant128);
          wide = false;
        }
      }
    }
  }

  // Promotion
  void promote() {
    if (!wide) {
      mant128 = static_cast<uInt128>(mant64);
      wide = true;
    }
  }

  // Demotion attempt (explicit)
  void try_demote() {
    if (wide && exp <= 63) {
      uint64_t hi = static_cast<uint64_t>(mant128 >> 64);
      if (hi == 0) {
        mant64 = static_cast<uint64_t>(mant128);
        wide = false;
      }
    }
  }

  // Multiply by 2^k (adjust exponent downward)
  void mul_pow2(uint32_t k) {
    if (k == 0 || is_zero()) return;
    if (!wide) {
      // Multiplying by 2^k is equivalent to decreasing exp by k, but we clamp at 0 and shift if needed.
      if (k <= exp) { exp -= k; return; }
      uint32_t needShift = k - exp;
      // Shift mantissa left; if it would overflow 64, promote
      if (needShift >= 64 || (mant64 >> (64 - needShift)) != 0) {
        promote();
        mant128 <<= needShift;
      } else {
        mant64 <<= needShift;
      }
      exp = 0;
    } else {
      if (k <= exp) { exp -= k; return; }
      mant128 <<= (k - exp);
      exp = 0;
    }
  }

  // Divide by 2^k (increase exponent => more fractional bits). This is always cheap.
  void div_pow2(uint32_t k) {
    if (k == 0 || is_zero()) return;
    exp += k;
    // If we crossed the 64-bit fractional budget, promote.
    if (!wide && exp > 63) promote();
  }

  // Divide by small uint64_t, auto-promote only if needed to go below 2^-64.
  // Ensures: if mant64 < d and we need extra fractional bits beyond 63, we promote and continue.
  void div_u64(uint64_t d) {
    // Precondition
    if (d == 0) { /* handle as needed (throw/assert) */ return; }
    if (is_zero()) return;

    if (!wide) {
      // Fast path: try to ensure numerator >= d by borrowing bits from the exponent, up to 63.
      if (mant64 < d) {
        if (mant64 == 0) { mant64 = 0; exp = 0; return; }
        int sd = flog2_u64(d);
        int sm = flog2_u64(mant64);
        uint32_t need = static_cast<uint32_t>((sd - sm) + 1); // minimum to make mant >= d
        uint32_t room = (exp <= 63) ? (63 - exp) : 0;
        uint32_t s = (need > room) ? room : need;
        if (s > 0) {
          mant64 <<= s;
          exp += s;
        }
      }

      if (mant64 >= d) {
        uint64_t q = mant64 / d;
        uint64_t r = mant64 % d;
        mant64 = q;
        // If remainder != 0 and we have room, we could optionally carry 1 extra bit by rounding; skip for speed.
        normalize();
        if (mant64 == 0 && r != 0) {
          // If we still fell to zero (rare), promote and re-run in 128.
          promote();
          mant128 = (static_cast<uInt128>(r) << 64) / static_cast<uInt128>(d);
          exp += 64;
          normalize();
        }
        return;
      }

      // We exhausted 64-bit fractional headroom; promote to preserve sub-2^-64 precision.
      promote();
      // Fallthrough into wide handling, with mant128 set.
      mant128 = static_cast<uInt128>(mant64);
    }

    // Wide path: we can always borrow more fractional bits by shifting in 128.
    if (mant128 < d) {
      // Shift enough to ensure mant128 >= d, but keep shifts bounded.
      int sd = flog2_u64(d);
      // mant128's floor_log2
      int sm = (mant128 >> 64) ? (64 + flog2_u64(static_cast<uint64_t>(mant128 >> 64)))
                               : flog2_u64(static_cast<uint64_t>(mant128));
      uint32_t need = static_cast<uint32_t>((sd - sm) + 1);
      if (need > 0) {
        mant128 <<= need;
        exp += need;
      }
    }

    mant128 /= static_cast<uInt128>(d);
    normalize(); // may demote if it fits and exp <= 63
  }

  // Multiply by uint64_t with overflow-aware promotion
  void mul_u64(uint64_t m) {
    if (m == 0 || is_zero()) { wide = false; mant64 = 0; mant128 = 0; exp = 0; return; }
    if (!wide) {
#if defined(__has_builtin)
  #if __has_builtin(__builtin_umull_overflow)
      unsigned __int128 tmp = static_cast<unsigned __int128>(mant64) * m;
      uint64_t hi = static_cast<uint64_t>(tmp >> 64);
      if (hi != 0) {
        promote();
        mant128 = static_cast<uInt128>(tmp);
      } else {
        mant64 = static_cast<uint64_t>(tmp);
      }
  #else
      // Portable check
      uint64_t a = mant64, b = m;
      if (a && b > std::numeric_limits<uint64_t>::max() / a) {
        promote();
        mant128 = static_cast<uInt128>(a) * static_cast<uInt128>(b);
      } else {
        mant64 = a * b;
      }
  #endif
#else
      // Fallback overflow check
      uint64_t a = mant64, b = m;
      if (a && b > std::numeric_limits<uint64_t>::max() / a) {
        promote();
        mant128 = static_cast<uInt128>(a) * static_cast<uInt128>(b);
      } else {
        mant64 = a * b;
      }
#endif
      normalize();
      return;
    }
    mant128 *= static_cast<uInt128>(m);
    normalize();
  }

}