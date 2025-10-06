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
    template<unsigned TotalBits>
    using TypeBasedOnBits =
        std::conditional_t<TotalBits <= 32, std::uint32_t,
        std::conditional_t<TotalBits <= 64, std::uint64_t,
    #if defined(__SIZEOF_INT128__)
        std::conditional_t<TotalBits <= 128, unsigned __int128,
    #else
        std::conditional_t<TotalBits <= 128, UInt64ArrayFallBack<2>, // fallback if no 128-bit
    #endif
        UInt64ArrayFallBack<(BitsCeil + 63) / 64>>>>;
				
    // Native-word detection (effective only if PackedT is a single integer type)
    template<typename T>
    constexpr bool is_native_word_v =
        std::is_same_v<T, std::uint32_t> ||
        std::is_same_v<T, std::uint64_t>
    #if defined(__SIZEOF_INT128__)
        || std::is_same_v<T, unsigned __int128>
    #endif
        ;
    
    template<typename T>
    constexpr unsigned bit_width_of_v = sizeof(T) * 8;
	}
}