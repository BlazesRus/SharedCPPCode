// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\DLLAPI.h" // Needed for compiling into DLL for usage
#include <array>
#include <cstdint>
#include <compare> // spaceship operator

// CPU carry/borrow intrinsics toggle
#if defined(_MSC_VER) && defined(_M_X64)
  #include <intrin.h>
  #define BlazesRus_HAVE_AddSubIntrinsics 1
#elif defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))
  #include <immintrin.h>
  #define BlazesRus_HAVE_AddSubIntrinsics 1
#else
  #define BlazesRus_HAVE_AddSubIntrinsics 0
#endif

namespace BlazesRusCode {

template <std::size_t N, bool UseIntrinsics = false>
struct DLLAPI UInt64ArrayFallback {
  static_assert(N > 0, "UInt64ArrayFallback must have at least one limb");

  // limbs[0] = least significant 64 bits; limbs[N-1] = most significant 64 bits
  std::array<std::uint64_t, N> limbs{};

  // ----- Construction from small integers -----

  // Construct from unsigned int (assumed <= 32-bit, but works for wider)
  constexpr UInt64ArrayFallback(unsigned int v) noexcept {
    limbs.fill(0);
    limbs[0] = static_cast<std::uint64_t>(v);
  }

  // Construct from uint64_t
  constexpr UInt64ArrayFallback(std::uint64_t v) noexcept {
    limbs.fill(0);
    limbs[0] = v;
  }

  // Default constructor
  constexpr UInt64ArrayFallback() noexcept = default;

  // Assignment from unsigned int
  constexpr UInt64ArrayFallback& operator=(unsigned int v) noexcept {
    limbs.fill(0);
    limbs[0] = static_cast<std::uint64_t>(v);
    return *this;
  }

  // Assignment from uint64_t
  constexpr UInt64ArrayFallback& operator=(std::uint64_t v) noexcept {
    limbs.fill(0);
    limbs[0] = v;
    return *this;
  }

  // Optional: narrowing conversion to unsigned int (lower limb only)
  // Note: ignores higher limbs; use with care.
  explicit constexpr operator unsigned int() const noexcept {
    return static_cast<unsigned int>(limbs[0]);
  }

  // ----- Equality and bitwise ops -----

  friend constexpr bool operator==(const UInt64ArrayFallback& a,
                                   const UInt64ArrayFallback& b) noexcept {
    return a.limbs == b.limbs;
  }

  friend constexpr UInt64ArrayFallback operator&(const UInt64ArrayFallback& a,
                                                 const UInt64ArrayFallback& b) noexcept {
    UInt64ArrayFallback r;
    for (std::size_t i = 0; i < N; ++i) r.limbs[i] = a.limbs[i] & b.limbs[i];
    return r;
  }

  friend constexpr UInt64ArrayFallback operator|(const UInt64ArrayFallback& a,
                                                 const UInt64ArrayFallback& b) noexcept {
    UInt64ArrayFallback r;
    for (std::size_t i = 0; i < N; ++i) r.limbs[i] = a.limbs[i] | b.limbs[i];
    return r;
  }

  friend constexpr UInt64ArrayFallback operator^(const UInt64ArrayFallback& a,
                                                 const UInt64ArrayFallback& b) noexcept {
    UInt64ArrayFallback r;
    for (std::size_t i = 0; i < N; ++i) r.limbs[i] = a.limbs[i] ^ b.limbs[i];
    return r;
  }

  // ----- Numeric spaceship: compare MS limb first -----

  friend constexpr std::strong_ordering
  operator<=>(const UInt64ArrayFallback& a, const UInt64ArrayFallback& b) noexcept {
    for (std::size_t i = N; i-- > 0; ) {
      if (a.limbs[i] < b.limbs[i]) return std::strong_ordering::less;
      if (a.limbs[i] > b.limbs[i]) return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
  }

  // ----- Shifts -----

  constexpr UInt64ArrayFallback& operator<<=(std::size_t k) noexcept {
    const std::size_t wordShift = k / 64;
    const std::size_t bitShift  = k % 64;

    if (wordShift >= N) {
      limbs.fill(0);
      return *this;
    }

			// shift whole words
    if (wordShift > 0) {
      for (std::size_t i = N; i-- > wordShift; ) {
        limbs[i] = limbs[i - wordShift];
      }
      for (std::size_t i = 0; i < wordShift; ++i) limbs[i] = 0;
    }

			// shift intra‑word
    if (bitShift > 0) {
      for (std::size_t i = N; i-- > 0; ) {
        std::uint64_t hi = (i > 0) ? (limbs[i - 1] >> (64 - bitShift)) : 0;
        limbs[i] = (limbs[i] << bitShift) | hi;
      }
    }
    return *this;
  }

	// Shift right by k bits
  constexpr UInt64ArrayFallback& operator>>=(std::size_t k) noexcept {
    const std::size_t wordShift = k / 64;
    const std::size_t bitShift  = k % 64;

    if (wordShift >= N) {
      limbs.fill(0);
      return *this;
    }

		// shift whole words
    if (wordShift > 0) {
      for (std::size_t i = 0; i + wordShift < N; ++i) {
        limbs[i] = limbs[i + wordShift];
      }
      for (std::size_t i = N - wordShift; i < N; ++i) {
        limbs[i] = 0;
      }
    }

		// shift intra‑word
    if (bitShift > 0) {
      for (std::size_t i = 0; i < N; ++i) {
        std::uint64_t lo = (i + 1 < N) ? (limbs[i + 1] << (64 - bitShift)) : 0;
        limbs[i] = (limbs[i] >> bitShift) | lo;
      }
    }
    return *this;
  }

  // ----- Carry/borrow helpers -----

  constexpr void propagate_carry_from(std::size_t i) noexcept {
    std::uint64_t carry = 1;
    for (std::size_t j = i; j < N && carry; ++j) {
      const std::uint64_t before = limbs[j];
      limbs[j] += carry;
      carry = (limbs[j] < before);
    }
  }

  constexpr void propagate_borrow_from(std::size_t i) noexcept {
    std::uint64_t borrow = 1;
    for (std::size_t j = i; j < N && borrow; ++j) {
      const std::uint64_t before = limbs[j];
      limbs[j] -= borrow;
      borrow = (limbs[j] > before);
    }
  }

  // ----- Addition (two overloads with requires) -----

  // Portable path: always viable unless intrinsics are both requested and available
  friend constexpr UInt64ArrayFallback
  operator+(UInt64ArrayFallback a, const UInt64ArrayFallback& b) noexcept
    requires (!UseIntrinsics || !BlazesRus_HAVE_AddSubIntrinsics)
  {
    std::uint64_t carry = 0;
    for (std::size_t i = 0; i < N; ++i) {
      const std::uint64_t ai = a.limbs[i];
      const std::uint64_t bi = b.limbs[i];

      const std::uint64_t t   = ai + bi;
      const bool carry1       = (t < ai);

      const std::uint64_t sum = t + carry;
      const bool carry2       = (sum < t);

      a.limbs[i] = sum;
      carry = static_cast<std::uint64_t>(carry1 || carry2);
    }
    return a;
  }

  // Intrinsics path: only viable if both template param and macro are true
  friend inline UInt64ArrayFallback
  operator+(UInt64ArrayFallback a, const UInt64ArrayFallback& b) noexcept
    requires (UseIntrinsics && BlazesRus_HAVE_AddSubIntrinsics)
  {
    unsigned char carry = 0;
    for (std::size_t i = 0; i < N; ++i) {
      carry = _addcarry_u64(carry, a.limbs[i], b.limbs[i], &a.limbs[i]);
    }
    return a;
  }

  // ----- Subtraction (two overloads with requires) -----

  // Portable path
  friend constexpr UInt64ArrayFallback
  operator-(UInt64ArrayFallback a, const UInt64ArrayFallback& b) noexcept
    requires (!UseIntrinsics || !BlazesRus_HAVE_AddSubIntrinsics)
  {
    std::uint64_t borrow = 0;
    for (std::size_t i = 0; i < N; ++i) {
      const std::uint64_t ai = a.limbs[i];
      const std::uint64_t bi = b.limbs[i];

      const std::uint64_t t       = ai - bi;
      const bool borrow1          = (ai < bi);
      const std::uint64_t diff    = t - borrow;
      const bool borrow2          = (t < borrow);

      a.limbs[i] = diff;
      borrow = static_cast<std::uint64_t>(borrow1 || borrow2);
    }
    return a;
  }

  // Intrinsics path
  friend inline UInt64ArrayFallback
  operator-(UInt64ArrayFallback a, const UInt64ArrayFallback& b) noexcept
    requires (UseIntrinsics && BlazesRus_HAVE_AddSubIntrinsics)
  {
    unsigned char borrow = 0;
    for (std::size_t i = 0; i < N; ++i) {
      borrow = _subborrow_u64(borrow, a.limbs[i], b.limbs[i], &a.limbs[i]);
    }
    return a;
  }
  
  // ----- Constants and predicates -----
  
  static constexpr UInt64ArrayFallback zero() noexcept {
      UInt64ArrayFallback r;
      r.limbs.fill(0);
      return r;
  }
  
  static constexpr UInt64ArrayFallback one() noexcept {
      UInt64ArrayFallback r;
      r.limbs.fill(0);
      r.limbs[0] = 1;
      return r;
  }
  
  constexpr bool is_zero() const noexcept {
      for (auto limb : limbs) if (limb != 0) return false;
      return true;
  }
  
  constexpr bool is_one() const noexcept {
      if (limbs[0] != 1) return false;
      for (std::size_t i = 1; i < N; ++i) if (limbs[i] != 0) return false;
      return true;
  }
  
  // ----- Increment / Decrement -----
  
  constexpr UInt64ArrayFallback& operator++() noexcept {
      propagate_carry_from(0);
      return *this;
  }
  constexpr UInt64ArrayFallback operator++(int) noexcept {
      auto tmp = *this;
      ++(*this);
      return tmp;
  }
  
  constexpr UInt64ArrayFallback& operator--() noexcept {
      propagate_borrow_from(0);
      return *this;
  }
  constexpr UInt64ArrayFallback operator--(int) noexcept {
      auto tmp = *this;
      --(*this);
      return tmp;
  }
  
  // ----- Logical predicates -----
  
  constexpr bool any() const noexcept {
      for (auto limb : limbs) if (limb != 0) return true;
      return false;
  }
  
  constexpr bool all_ones() const noexcept {
      for (auto limb : limbs) if (limb != UINT64_MAX) return false;
      return true;
  }
  
  // ----- Bit access helpers -----
  
  constexpr bool test_bit(std::size_t pos) const noexcept {
      const std::size_t word = pos / 64;
      const std::size_t bit  = pos % 64;
      return (word < N) ? ((limbs[word] >> bit) & 1ULL) : false;
  }
  
  constexpr void set_bit(std::size_t pos) noexcept {
      const std::size_t word = pos / 64;
      const std::size_t bit  = pos % 64;
      if (word < N) limbs[word] |= (1ULL << bit);
  }
  
  constexpr void clear_bit(std::size_t pos) noexcept {
      const std::size_t word = pos / 64;
      const std::size_t bit  = pos % 64;
      if (word < N) limbs[word] &= ~(1ULL << bit);
  }
  
  // ----- Scalar add/sub -----
  
  friend constexpr UInt64ArrayFallback
  operator+(UInt64ArrayFallback a, std::uint64_t b) noexcept {
      std::uint64_t carry = b;
      for (std::size_t i = 0; i < N && carry; ++i) {
          const std::uint64_t before = a.limbs[i];
          a.limbs[i] += carry;
          carry = (a.limbs[i] < before);
      }
      return a;
  }
  
  friend constexpr UInt64ArrayFallback
  operator-(UInt64ArrayFallback a, std::uint64_t b) noexcept {
      std::uint64_t borrow = b;
      for (std::size_t i = 0; i < N && borrow; ++i) {
          const std::uint64_t before = a.limbs[i];
          a.limbs[i] -= borrow;
          borrow = (a.limbs[i] > before);
      }
      return a;
  }
};

// Non-mutating shifts (free functions)
template <std::size_t N, bool UseIntrinsics>
constexpr UInt64ArrayFallback<N, UseIntrinsics>
operator<<(UInt64ArrayFallback<N, UseIntrinsics> lhs, std::size_t k) noexcept {
  lhs <<= k;
  return lhs;
}

template <std::size_t N, bool UseIntrinsics>
constexpr UInt64ArrayFallback<N, UseIntrinsics>
operator>>(UInt64ArrayFallback<N, UseIntrinsics> lhs, std::size_t k) noexcept {
  lhs >>= k;
  return lhs;
}

template<typename T>
concept UInt64ArrayFallbackVariant = std::is_base_of<UInt64ArrayFallback, T>::value;

template<typename>
struct is_UInt64ArrayFallbackVariant : std::false_type {};

template<std::size_t N, bool UseIntrinsics>
struct is_UInt64ArrayFallbackVariant<UInt64ArrayFallback<N, UseIntrinsics>> : std::true_type {};

template<typename T>
concept UInt64ArrayFallbackVariant = is_UInt64ArrayFallbackVariant<T>::value;

} // namespace BlazesRusCode
