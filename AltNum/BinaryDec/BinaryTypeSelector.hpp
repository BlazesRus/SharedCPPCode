// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <type_traits>
#include <cstddef>
#include <string>
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
    
		template <unsigned PrecisionLevel:1>
		constexpr unsigned bits_required_from_value(UInt v) {
				static_assert(std::is_unsigned_v<UInt>, "Unsigned type required");
				if (v == 0) return 0;
				return std::numeric_limits<UInt>::digits - std::countl_zero(v);
		}
		
		// Storage bit width (useful for native words only; not a semantic ceiling)
    template<typename T>
    constexpr unsigned bit_width_of_v = sizeof(T) * 8;
		
    template<typename T>
		constexpr unsigned required_bits(std::string_view digits) {
		//Requires format to not have ' etc inside string
		//ToDo:Cases 3+ are just skeletons to finish later
				switch (digits.size()) {
				case 0:
						return 0;
				case 1: {
						char currentDigit = digits[0];
						//if (currentDigit == '8') return 4;//Exact Bit of 8
						if(currentDigit>=5) return 4;
						//if (currentDigit == '4') return 3;//Exact Bit of 4
						if (currentDigit >= '3') return 3;//Rounds up to next bit (2.5 Bits needed at 3)
						if (currentDigit == '2') return 2;
						if (currentDigit == '1') return 1;
						throw("Invalid digit");//This should not be reached unless string has non-numeric character
				}

				case 2: {
						char currentDigit = digits[0];
						if (currentDigit > '6')
						  return 8;
						else if (currentDigit == '6'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
								//if(currentDigit==4) return 7;//Exact Bit of 64
								if(currentDigit>=5) return 8;//Round up to next bit
						    else
								  return 7;//Round up to next bit
							} else
							  return 8;//Round up to highest bit requirement
						}
						else if(currentDigit > '3')
						  return 7;//Round up to next bit
						else if (currentDigit == '3'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
								//if(currentDigit==2) return 6;//Exact Bit of 32
								if(currentDigit>=3) return 7;//Round up to next bit
						    else
								  return 6;//Round up to next bit
							} else
							  return 7;//Round up to highest bit requirement
						}
						else if(currentDigit > '1')
						  return 5;//Rounded up to next bit
						else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
								//if(currentDigit==6) return 5;//Exact Bit of 16
								if(currentDigit>=7) return 6;//Round up to next bit
						    else
								  return 5;//Round up to next bit
							} else
							  return 6;//Round up to highest bit requirement
						}
						//0X formatting is invalid
						//else if(currentDigit== '0' )
						//  return 4;//Rounded up to next bit
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}

				case 3: {
						char currentDigit = digits[0];
						if (currentDigit > '5')
						  return ?;
						else if (currentDigit == '5'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						if (currentDigit > '2')
						  return ?;
						else if (currentDigit == '2'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
/*
					128(8): X-? (to Y/X)
					256(9): X-? (to Y/X)
					512(10): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 4: {
						char currentDigit = digits[0];
						if (currentDigit > '8')
						  return ?;
						else if (currentDigit == '8'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit > '4')
						  return ?;
						else if (currentDigit == '4'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
/*
					1'024(11): X-? (to Y/X)
					2'048(12): X-? (to Y/X)
					4'096(13): X-? (to Y/X)
					8'192(14): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 5: {
						char currentDigit = digits[0];
						if (currentDigit > '6')
						  return ?;
						else if (currentDigit == '6'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit > '3')
						  return ?;
						else if (currentDigit == '3'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
/*
					16'384(15): X-? (to Y/X)
					32'768(16): X-? (to Y/X)
					65'536(17): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 6: {
						char currentDigit = digits[0];
						if (currentDigit > '5')
						  return ?;
						else if (currentDigit == '5'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit > '2')
						  return ?;
						else if (currentDigit == '2'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
						else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
							  currentDigit = digits[1];
                if constexpr (PrecisionLevel >= 2) {
								  currentDigit = digits[1];
								}
								else
								  return X;
							} else
							  return X;
						}
/*
					131'072(18): X-? (to Y/X)
					262'144(19): X-? (to Y/X)
					524'288(20): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 7: {
						char currentDigit = digits[0];
/*
					1'048'576(21): X-? (to Y/X)
					2'097'152(22): X-? (to Y/X)
					4'194'304(23): X-? (to Y/X)
					8'388'608(24): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 8: {
						char currentDigit = digits[0];
/*
					16'777'216(25): X-? (to Y/X)
					33'554'432(26): X-? (to Y/X)
					67'108'864(27): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 9: {
						char currentDigit = digits[0];
/*
					134'217'728(28): X-? (to Y/X)
					268'435'456(29): X-? (to Y/X)
					536'870'912(30): X-? (to Y/X)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				case 10: {
						char currentDigit = digits[0];
/*
					1'073'741'824(31): X-? (to Y/X)
					2'147'483'648(32): 2'147'483'648-4'294'967'295 (to 2'147'483'647 / 2'147'483'648)
					4'294'967'295 = uint32 maximum(Exactly 32 full bits--Prevent rounding to 33 bits if under 4294967295)
					4'294'967'296(33): 4'294'967'296-8'589'934'591 (to 4'294'967'295/4'294'967'296)
*/
						throw("Invalid digit format");//This should not be reached unless formatting is invalid
				}
				default:
						throw("Unsupported string length parsed");
				}
		}
  }
		
		static constexpr bool IsValueEffective = ValueSafe ?
		(UseCustomMode? (BinaryDecCode::is_native_word_v<PackedT> && (INT_BITS + DEC_BITS) <= BinaryDecCode::bit_width_of_v<PackedT>)
		: true):false;
	}
}