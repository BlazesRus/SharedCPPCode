// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <cstdint>
#include <limits>
#include <bit>    // C++20 std::bit_width
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

namespace BlazesRusCode
{
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
	
  struct NoExtraRep {};

	template<typename Policy>
	struct ExtraRepFields
	{
		using ExtraRepT = typename Policy::ExtraRepT;
		ExtraRepT ExtraRep;

		static inline constexpr bool EnableExtraRepFlags =
			Policy::EnablePowerOf || Policy::EnableMixedFrac;
		
		static constexpr unsigned MixedFracDivisorLimit =
			(Policy::EnableMixedFrac && EnableExtraRep)
				? (EnableMaskedExtraRep && EXTRA_BITS > 0
					? ((EXTRA_BITS >= 31) ? 0x7FFFFFFFu : ((1u << EXTRA_BITS) - 1u))
					: Policy::MixedFracDivisorLimit) // from plain ExtraRep
		: 0;
		
		constexpr void EnforceExtraRepShapeIfAltRep()
		{
				if constexpr (IsExtraRepUsingBitMasks) {
						static_assert(
								CheckMemberOrMethodExistance::HasValueMethodAndIsAltRepMethod<typename Policy::ExtraRepT>,
								"In AltRep (PowerOf/MixedFrac) mode, ExtraRepT must provide Value() and IsAltRep() methods."
						);
				}
				else if constexpr (Policy::EnableExtraRep) {
						static_assert(
								CheckMemberOrMethodExistance::HasValueMemberAndIsAltRepMember<typename Policy::ExtraRepT>,
								"In Non-Masked ExtraRep mode, ExtraRepT must have Value and IsAltRep members."
						);
				}
		}

		EnforceExtraRepShapeIfAltRep();

		// ctor / init to zero or whatever you like
		ExtraRepFields() noexcept : ExtraRep{} {}

		// optional helper to normalize zero-sentinel
		void normalizeZero() noexcept {
			if constexpr(std::is_integral_v<ExtraRepT>) {
				if (ExtraRep == ExtraRepT{}) {
					// already zero
				}
				else if (/* both halves are zero? need access to them */) {
					ExtraRep = ExtraRepT{};
				}
			}
		}
	};
	
}