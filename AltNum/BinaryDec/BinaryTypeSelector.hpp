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
    
		/*
				Corridor digit walk:
					- PrecisionLevel == 0 → treat as maximum precision (walk all digits).
					- PrecisionLevel > 0 → walk up to that many digits, then conservatively
						upgrade if undecided before the last digit.
				Return value is the minimum number of bits required at the current precision level.
		*/
		template<unsigned int PrecisionLevel, int N>
		constexpr unsigned int stringBitsCorridor(std::string_view digits,
																							const char (&ceiling)[N],
																							unsigned int X, unsigned int Y) {
			const unsigned int LastIndex = N - 1;

			// Normalize: 0 = unlimited
			const unsigned int effectivePrecision =
					(PrecisionLevel == 0) ? LastIndex : PrecisionLevel;

			unsigned int digitIndex = 1; // skip leading digit (handled outside)

			for (unsigned int ceilingIndex = 1;
					 ceilingIndex < LastIndex && digitIndex < effectivePrecision;
					 ++ceilingIndex) {
					char ceilingDigit = ceiling[ceilingIndex];
					if (ceilingDigit == '\'') continue; // skip separators

					char currentDigit = digits[digitIndex++];
					if (currentDigit > ceilingDigit) return Y;
					if (currentDigit < ceilingDigit) return X;
			}

			if (effectivePrecision >= LastIndex) {
					// Last digit in ceiling is guaranteed numeric if formatted correctly
					char currentDigit = digits.back();
					return (currentDigit >= ceiling[LastIndex]) ? Y : X;
			}

			return Y; // conservative fallback
		}
		
		template<unsigned int PrecisionLevel, int N>
		constexpr bool stringBitsDigitsCorridor(std::string_view digits,
																						const char (&ceiling)[N],
																						unsigned int X, unsigned int Y,
																						unsigned int& out) {
				if (digits[0] > ceiling[0]) {
						out = Y;
						return true;   // decision made
				}
				if (digits[0] == ceiling[0]) {
						out = stringBitsCorridor<PrecisionLevel>(digits, ceiling, X, Y);
						return true;   // decision made
				}
				// digits[0] < ceiling[0] → not this corridor, fall through
				return false;
		}
		
		//Minimum PrecisionLevel is treated as 1, if PrecisionLevel==0 then at maximum precision level
    template<unsigned int PrecisionLevel=0>
    constexpr unsigned required_bits(std::string_view digits) {
    //Requires format to not have ' etc inside string
    //Between top bits it costs same as top bit minimum
    //ToDo:Cases 3+ are just skeletons to finish later
      switch (digits.size()) {
        case 0:
            return 0;
        case 1: {
            char currentDigit = digits[0];
            if (currentDigit >= '8') return 4;//Exact Top Bit of 8
            else if (currentDigit >= '4') return 3;//Exact Top Bit of 4
            else if (currentDigit >= '2') return 2;
            else if (currentDigit == '1') return 1;
            return 0;
        }

        case 2: {
            char currentDigit = digits[0];
            if (currentDigit > '6')
              return 7;
            else if (currentDigit == '6'){
							currentDigit = digits[1];
							if(currentDigit>=4) return 7;//Exact Bit of 64
							else
								return 6;
            }
            else if(currentDigit > '3')
              return 6;//Round up to next bit
            else if (currentDigit == '3'){
							currentDigit = digits[1];
							if(currentDigit>=2) return 6;//Exact Bit of 32
							else
								return 5;
            }
            else if(currentDigit > '1')
              return 5;
            else if (currentDigit == '1'){
							currentDigit = digits[1];
							if(currentDigit>=6) return 5;//Exact Bit of 16
							else
								return 4;
            }
            //0X formatting is invalid
            throw std::runtime_error("Invalid digit format");//This should not be reached unless formatting is invalid
        /*
          16(5): 16-31 (to 15/16)
          32(6): 32-63 (to Y/X)
          64(7): 64-127 (to Y/X)
        */
        }
				case 3: {
					char currentDigit = digits[0];
					if (currentDigit > '5')
						return 10;
					else if (currentDigit == '5')
						return stringBitsCorridor<PrecisionLevel>(digits, "512", 9, 10);
					else if (currentDigit > '2')
						return 9;
					else if (currentDigit == '2')
						return stringBitsCorridor<0>(digits, "256", 8, 9);//Uint8
					else if (currentDigit == '1')
						return stringBitsCorridor<PrecisionLevel>(digits, "128", 7, 8);
					throw std::runtime_error("Invalid digit format"); // Should not be reached unless formatting is invalid
				}
        case 4: {
					char currentDigit = digits[0];
					if (currentDigit > '8')
						return 14;
					else if (currentDigit == '8')
            return stringBitsCorridor<PrecisionLevel>(digits, "8'192", 13, 14);
					else if (currentDigit > '4')
						return 13;
					else if (currentDigit == '4')
            return stringBitsCorridor<PrecisionLevel>(digits, "4'096", 12, 13);
					else if (currentDigit=='3')
						return 12;
					else if (currentDigit == '2')
            return stringBitsCorridor<PrecisionLevel>(digits, "2'048", 11, 12);
					else if (currentDigit == '1')
            return stringBitsCorridor<PrecisionLevel>(digits, "1'024", 10, 11);
          throw std::runtime_error("Invalid digit format");//This should not be reached unless formatting is invalid
        }
        case 5: {
					char currentDigit = digits[0];
					if (currentDigit > '6')
						return 17;
					else if (currentDigit == '6')
					  return stringBitsCorridor<0>(digits, "65'536", 16, 17);//Uint16
					else if (currentDigit > '3')
						return 16;
					else if (currentDigit == '3')
            return stringBitsCorridor<PrecisionLevel>(digits, "32'768", 15, 16);
					else if (currentDigit > '1')
						return 15;
					else if (currentDigit == '1')
            return stringBitsCorridor<PrecisionLevel>(digits, "16'384", 14, 15);
          throw std::runtime_error("Invalid digit format");//This should not be reached unless formatting is invalid
        }
        case 6: {
					unsigned int result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "524'288", 19, 20, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "262'144", 18, 19, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "131'072", 17, 18, result)) return result;
					throw std::runtime_error("Invalid digit format");
        }
        case 7: {
					unsigned int result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "8'388'608", 23, 24, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "4'194'304", 22, 23, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "2'097'152", 21, 22, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "1'048'576", 20, 21, result)) return result;
					throw std::runtime_error("Invalid digit format");
        }
        case 8: {
					unsigned int result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "67'108'864", 26, 27, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "33'554'432", 25, 26, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "16'777'216", 24, 25, result)) return result;
					throw std::runtime_error("Invalid digit format");
        }
        case 9: {
					unsigned int result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "536'870'912", 29, 30, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "268'435'456", 28, 29, result)) return result;
					if (stringBitsDigitsCorridor<PrecisionLevel>(digits, "134'217'728", 27, 28, result)) return result;
					throw std::runtime_error("Invalid digit format");
        }
        case 10: {
					char currentDigit = digits[0];
					if (currentDigit > '8')
						return 34;
					else if (currentDigit == '8')
					  return stringBitsCorridor<0>(digits, "8'589'934'592", 33, 34);
					else if (currentDigit > '4')
						return 33;
					else if (currentDigit == '4')
            return stringBitsCorridor<0>(digits, "4'294'967'296", 32, 33);//UInt 32
					else if (currentDigit > '2')
						return 32;
					else if (currentDigit == '2')
            return stringBitsCorridor<0>(digits, "2'147'483'648", 31, 32);//Signed int 32
					else if (currentDigit == '1')
            return stringBitsCorridor<PrecisionLevel>(digits, "1'073'741'824", 30, 31);
          /*
            1'073'741'824(31): X-? (to Y/X)
            2'147'483'648(32): 2'147'483'648-4'294'967'295 (to 2'147'483'647 / 2'147'483'648)
            4'294'967'295 = uint32 maximum(Exactly 32 full bits--Prevent rounding to 33 bits if under 4294967295)
            4'294'967'296(33): 4'294'967'296-8'589'934'591 (to 4'294'967'295/4'294'967'296)
            8'589'934'592(34): X-? (to Y/X)
          */
          throw std::runtime_error("Invalid digit format");//This should not be reached unless formatting is invalid
        }
//        case 11: {
//            char currentDigit = digits[0];
/*
        17'179'869'184(35): X-? (to Y/X)
        34'359'738'368(36): X-? (to Y/X)
        68'719'476'736(37): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 12: {
//            char currentDigit = digits[0];
/*
        137'438'953'472(38): X-? (to Y/X)
        274'877906944(39): X-? (to Y/X)
        549'755813888(40): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 13: {
//            char currentDigit = digits[0];
/*
        1'099'511627776(41): X-? (to Y/X)
        2'199'023255552(42): X-? (to Y/X)
        4'398'046511104(43): X-? (to Y/X)
        8'796'093022208(44): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 14: {
//            char currentDigit = digits[0];
/*
        17'592'186044416(45): X-? (to Y/X)
        35'184372088832(46): X-? (to Y/X)
        70'368744177664(47): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 15: {
//            char currentDigit = digits[0];
/*
        140'737488355328(48): X-? (to Y/X)
        281'474976710656(49): X-? (to Y/X)
        562'949953421312(50): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 16: {
//            char currentDigit = digits[0];
/*
        1'125'899906842624(51): X-? (to Y/X)
        2'251'799813685248(52): X-? (to Y/X)
        4'503'599627370496(53): X-? (to Y/X)
        9'007'199254740992(54): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 17: {
//            char currentDigit = digits[0];
/*
        18'014'398509481984(55): X-? (to Y/X)
        36'028'797018963968(56): X-? (to Y/X)
        72'057'594037927936(57): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 18: {
//            char currentDigit = digits[0];
/*
        144'115'188075855872(58): X-? (to Y/X)
        288'230'376151711744(59): X-? (to Y/X)
        576'460'752303423488(60): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 19: {
//            char currentDigit = digits[0];
/*
        1'152'921504606846976(61): X-? (to Y/X)
        2'305'843009213693952(62): X-? (to Y/X)
        4'611'686018427387904(63): X-? (to Y/X)
        9'223'372036854775808(64): X-? (to Y/9223372036854775808)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 20: {
//            char currentDigit = digits[0];
/*
        18'446'744073709551616(65): X-? (to Y/X)
*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
//        case 21: {
//            char currentDigit = digits[0];
/*

*/
//            throw("Invalid digit format");//This should not be reached unless formatting is invalid
//        }
        default:
            throw("Unsupported string length parsed");
        }
    }
    
    static constexpr bool IsValueEffective = ValueSafe ?
    (UseCustomMode? (BinaryDecCode::is_native_word_v<PackedT> && (INT_BITS + DEC_BITS) <= BinaryDecCode::bit_width_of_v<PackedT>)
    : true):false;
  }
}