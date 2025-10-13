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
              if constexpr (PrecisionLevel >= 1) {
                currentDigit = digits[1];
                if(currentDigit>=4) return 7;//Exact Bit of 64
                else
                  return 6;
              } else
                return 7;//Round up to highest bit requirement
            }
            else if(currentDigit > '3')
              return 6;//Round up to next bit
            else if (currentDigit == '3'){
              if constexpr (PrecisionLevel >= 1) {
                currentDigit = digits[1];
                if(currentDigit>=2) return 6;//Exact Bit of 32
                else
                  return 5;
              } else
                return 6;
            }
            else if(currentDigit > '1')
              return 5;
            else if (currentDigit == '1'){
              if constexpr (PrecisionLevel >= 1) {
                currentDigit = digits[1];
                if(currentDigit>=6) return 5;//Exact Bit of 16
                else
                  return 4;
              } else
                return 5;
            }
            //0X formatting is invalid
            throw("Invalid digit format");//This should not be reached unless formatting is invalid
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
            else if (currentDigit == '5'){

            } else if (currentDigit > '2')
              return 9;
            else if (currentDigit == '2'){

            } else if (currentDigit == '1'){

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
              return 14;
            else if (currentDigit == '8'){

            } else if (currentDigit > '4')
              return 13;
            else if (currentDigit == '4'){
						
            } else if (currentDigit=='3')
              return 12;
						else if (currentDigit == '2'){

            } else if (currentDigit == '1'){

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
              return 17;
            else if (currentDigit == '6'){

            } else if (currentDigit > '3')
              return 16;
            else if (currentDigit == '3'){

            } else if (currentDigit > '1')
              return 15;
            else if (currentDigit == '1'){

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
              return 20;
            else if (currentDigit == '5'){

            } else if (currentDigit > '2')
              return 19;
            else if (currentDigit == '2'){

            } else if (currentDigit == '1'){

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
            if (currentDigit > '8')
              return 24;
            else if (currentDigit == '8'){
						} else if (currentDigit > '4')
              return 23;
            else if (currentDigit == '4'){
						} else if (currentDigit > '2')
              return 22;
            else if (currentDigit == '2'){
						} else if (currentDigit > '1')
              return 21;
            else if (currentDigit == '1'){
						}
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
            if (currentDigit > '6')
              return 27;
            else if (currentDigit == '6'){
						} else if (currentDigit > '3')
              return 26;
            else if (currentDigit == '3'){
						} else if (currentDigit > '1')
              return 25;
            else if (currentDigit == '1'){
						}
/*
          16'777'216(25): X-? (to Y/X)
          33'554'432(26): X-? (to Y/X)
          67'108'864(27): X-? (to Y/X)
*/
            throw("Invalid digit format");//This should not be reached unless formatting is invalid
        }
        case 9: {
            char currentDigit = digits[0];
            if (currentDigit > '5')
              return 30;
            else if (currentDigit == '5'){
						}
            else if (currentDigit > '2')
              return 29;
            else if (currentDigit == '2'){
						}
            else if (currentDigit > '1')
              return 28;
            else if (currentDigit == '1'){
						}
/*
          134'217'728(28): X-? (to Y/X)
          268'435'456(29): X-? (to Y/X)
          536'870'912(30): X-? (to Y/X)
*/
            throw("Invalid digit format");//This should not be reached unless formatting is invalid
        }
        case 10: {
            char currentDigit = digits[0];
            if (currentDigit > '8')
              return 34;
            else if (currentDigit == '8'){
							if constexpr (PrecisionLevel >= 1){
								currentDigit = digits[1];
								if(currentDigit>'5') return 34;
								if(currentDigit<'5') return 33;
							}
							if constexpr (PrecisionLevel >= 2){
							  currentDigit = digits[2];
							  if(currentDigit>'8') return 34;
								if(currentDigit<'8') return 33;
							}
							if constexpr (PrecisionLevel >= 3){
							  currentDigit = digits[3];
							  if(currentDigit>'4') return 34;
								if(currentDigit<'4') return 33;
							}
							if constexpr (PrecisionLevel >= 4){
							  currentDigit = digits[4];
							  if(currentDigit>'9') return 34;
								if(currentDigit<'9') return 33;
							}
							if constexpr (PrecisionLevel >= 5){
							  currentDigit = digits[5];
							  if(currentDigit>'3') return 34;
								if(currentDigit<'3') return 33;
							}
							if constexpr (PrecisionLevel >= 6){
							  currentDigit = digits[6];
							  if(currentDigit>'4') return 34;
								if(currentDigit<'4') return 33;
							}
							if constexpr (PrecisionLevel >= 7){
							  currentDigit = digits[7];
							  if(currentDigit>'2') return 34;
								if(currentDigit<'2') return 33;
							}
							if constexpr (PrecisionLevel >= 8){
							  currentDigit = digits[8];
							  if(currentDigit>'9') return 34;
								if(currentDigit<'9') return 33;
							}
							if constexpr (PrecisionLevel >= 9){
							  currentDigit = digits[9];
							  if(currentDigit>='2') return 34;
							  return 33;
							}
							return 34;
            }
            else if (currentDigit > '4')
              return 33;
            else if (currentDigit == '4'){
						  //Forcing maximum precision of uint32 bit boundry
              currentDigit = digits[1];
							if(currentDigit>'2') return 33;
							if(currentDigit<'2') return 32;
							currentDigit = digits[2];
							if(currentDigit>'9') return 33;
							if(currentDigit<'9') return 32;
							currentDigit = digits[3];
							if(currentDigit>'4') return 33;
							if(currentDigit<'4') return 32;
							currentDigit = digits[4];
							if(currentDigit>'9') return 33;
							if(currentDigit<'9') return 32;
							currentDigit = digits[5];
							if(currentDigit>'6') return 33;
							if(currentDigit<'6') return 32;
							currentDigit = digits[6];
							if(currentDigit>'7') return 33;
							if(currentDigit<'7') return 32;
							currentDigit = digits[7];
							if(currentDigit>'2') return 33;
							if(currentDigit<'2') return 32;
							currentDigit = digits[8];
							if(currentDigit>'9') return 33;
							if(currentDigit<'9') return 32;
							currentDigit = digits[9];
							if(currentDigit>='6') return 33;
							return 32;
            }
            else if (currentDigit > '2')
              return 32;
            else if (currentDigit == '2'){
							if constexpr (PrecisionLevel >= 1){
								currentDigit = digits[1];
								if(currentDigit>'1') return 32;
								if(currentDigit=='0') return 31;
							}
							if constexpr (PrecisionLevel >= 2){
							  currentDigit = digits[2];
							  if(currentDigit>'4') return 32;
								if(currentDigit<'4') return 31;
							}
							if constexpr (PrecisionLevel >= 3){
							  currentDigit = digits[3];
							  if(currentDigit>'7') return 32;
								if(currentDigit<'7') return 31;
							}
							if constexpr (PrecisionLevel >= 4){
							  currentDigit = digits[4];
							  if(currentDigit>'4') return 32;
								if(currentDigit<'4') return 31;
							}
							if constexpr (PrecisionLevel >= 5){
							  currentDigit = digits[5];
							  if(currentDigit>'8') return 32;
								if(currentDigit<'8') return 31;
							}
							if constexpr (PrecisionLevel >= 6){
							  currentDigit = digits[6];
							  if(currentDigit>'3') return 32;
								if(currentDigit<'3') return 31;
							}
							if constexpr (PrecisionLevel >= 7){
							  currentDigit = digits[7];
							  if(currentDigit>'6') return 32;
								if(currentDigit<'6') return 31;
							}
							if constexpr (PrecisionLevel >= 8){
							  currentDigit = digits[8];
							  if(currentDigit>'4') return 32;
								if(currentDigit<'4') return 31;
							}
							if constexpr (PrecisionLevel >= 9){
							  currentDigit = digits[9];
							  if(currentDigit>='8') return 32;
							  return 31;
							}
							return 32;
            }
            else if (currentDigit == '1'){
							if constexpr (PrecisionLevel == 1)
							  return 31;
							else if constexpr (PrecisionLevel > 1){
								currentDigit = digits[1];
								if(currentDigit>'0') return 31;
							}
							if constexpr (PrecisionLevel >= 2){
							  currentDigit = digits[2];
							  if(currentDigit>'7') return 31;
							  if(currentDigit<'7') return 30;
							}
							if constexpr (PrecisionLevel >= 3){
							  currentDigit = digits[3];
							  if(currentDigit>'3') return 31;
							  if(currentDigit<'3') return 30;
							}
							if constexpr (PrecisionLevel >= 4){
							  currentDigit = digits[4];
							  if(currentDigit>'7') return 31;
							  if(currentDigit<'7') return 30;
							}
							if constexpr (PrecisionLevel >= 5){
							  currentDigit = digits[5];
							  if(currentDigit>'4') return 31;
							  if(currentDigit<'4') return 30;
							}
							if constexpr (PrecisionLevel >= 6){
							  currentDigit = digits[6];
							  if(currentDigit>'1') return 31;
							  if(currentDigit=='0') return 30;
							}
							if constexpr (PrecisionLevel >= 7){
							  currentDigit = digits[7];
							  if(currentDigit>'2') return 31;
							  if(currentDigit<'2') return 30;
							}
							if constexpr (PrecisionLevel >= 8){
							  currentDigit = digits[8];
							  if(currentDigit>'9') return 31;
							  if(currentDigit<'9') return 30;
							}
							if constexpr (PrecisionLevel >= 9){
							  currentDigit = digits[9];
							  if(currentDigit>='2') return 31;
							  return 30;
							}
							return 31;
            }
/*
          1'073'741'824(31): X-? (to Y/X)
          2'147'483'648(32): 2'147'483'648-4'294'967'295 (to 2'147'483'647 / 2'147'483'648)
          4'294'967'295 = uint32 maximum(Exactly 32 full bits--Prevent rounding to 33 bits if under 4294967295)
          4'294'967'296(33): 4'294'967'296-8'589'934'591 (to 4'294'967'295/4'294'967'296)
          8'589'934'592(34): X-? (to Y/X)
*/
            throw("Invalid digit format");//This should not be reached unless formatting is invalid
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