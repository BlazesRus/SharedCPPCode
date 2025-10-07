// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <type_traits>
#include <cstddef>
#include "UInt64ArrayFallback.hpp"

namespace BlazesRusCode
{
  namespace BinaryTypeSelector
  {
		template<std::size_t TotalBits>
		struct TypeForBits {
			static_assert(TotalBits > 0, "TotalBits must be greater than zero");

			using type =
				std::conditional_t<(TotalBits <= 32), std::uint32_t,
				std::conditional_t<(TotalBits <= 64), std::uint64_t,
		#if defined(__SIZEOF_INT128__)
				std::conditional_t<(TotalBits <= 128), unsigned __int128,
				UInt64ArrayFallback<(TotalBits + 63) / 64>
				>
		#else
				UInt64ArrayFallback<(TotalBits + 63) / 64>
		#endif
				>>;
		};

		template<std::size_t TotalBits>
		using TypeBasedOnBits = typename TypeForBits<TotalBits>::type;
				
    // Native-word detection (effective only if PackedT is a single integer type)
    template<typename T>
    constexpr bool is_native_word_v =
        std::is_same_v<T, std::uint32_t> ||
        std::is_same_v<T, std::uint64_t>
    #if defined(__SIZEOF_INT128__)
        || std::is_same_v<T, unsigned __int128>
    #endif
        ;
    
		// Storage bit width (useful for native words only; not a semantic ceiling)
    template<typename T>
    constexpr unsigned bit_width_of_v = sizeof(T) * 8;
	}
}