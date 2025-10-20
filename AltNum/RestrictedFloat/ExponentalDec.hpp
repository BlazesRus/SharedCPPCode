// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// Code adjusted with help of Microsoft Copilot
// ***********************************************************************
#pragma once

#include "RestrictedFloat.hpp"
#include <utility>
#include <vector>

//ExponentalDec variants
namespace BlazesRusCode {

template<unsigned unsigned fracBits>
struct ExponentalDecLayout {
  unsigned extendedRangeBits;
	unsigned maxOptimalExtendedRange;
	//Safe to use unsigned int as ExpMag because anything bigger is way too big of a result if use 2^X
	std::array<std::pair<unsigned int, BinaryTypeSelector::TypeForBits<fracBits>>, tmp.size()> extendedOffsetTable;
	std::pair<unsigned int, BinaryTypeSelector::TypeForBits<fracBits>> maximumEffective;
}

//Generates default 
template<unsigned magBits, unsigned unsigned fracBits>
struct ExponentalDecPolicy {
  static inline constexpr bool StandaloneMode = true;
	static inline constexpr bool SignedMode = true;
  static inline constexpr bool SignedExpMode = true;
  static inline constexpr bool EnablePi = true;
  static inline constexpr bool EnableE = true;
  static inline constexpr bool EnableI = true;
	static inline constexpr bool EnableZeroSentinal = true;
	static inline constexpr bool EnableExtraFeatures = true;

  //Estimate ExtendedRange needed to cover all ExtendedRange increments until offset starting offset becomes 
	constexpr int RequiredBits(uint32_t n) {
			int bits = 0;
			while (n > 0) {
					++bits;
					n >>= 1;
			}
			return bits;
	}

  static constexpr ExponentalDecLayout<fracBits> ComputeExponentalDecLayout() noexcept {
	  unsigned int totalRawBits = magBits+fracBits;
		unsigned int extendedBits;// = RequiredBits(totalRawBits);
    unsigned int StartingOffset = 1<<magBits;
		
    using ticksT  = BinaryTypeSelector::TypeForBits<totalRawBits+1>;
    ticksT gainTicks = StartingOffset<<fracBits;
		
    unsigned int expMagGain;
    //Give one extra bit for overflow
    using OverflowedFracT  = BinaryTypeSelector::TypeForBits<fracBits+1>;
    using FracT  = BinaryTypeSelector::TypeForBits<fracBits>;
		
    unsigned int expMag = 0;
    OverflowedFracT expFrac = 0;
    OverflowedFracT maxDenom = 1<<fracBits;
		
    unsigned int extendedRange = 0;
    using Pair  = std::pair<unsigned, FracT>;
		
		std::array<Pair, totalRawBits> extendedOffsetTable;
    //Adding dummy zero index so that starts range at 1
    tmp.push_back({0, 0});
    while (gainTicks > 0) {
      expMag += gainTicks >> fracBits;
      expFrac += FracT(gainTicks & ((ticksT(1u) << ticksT(fracBits)) - ticksT(1)));
      if(expFrac>=maxDenom){
        expFrac -= maxDenom;
        ++expMag;
      }
      extendedOffsetTable[i] = {expMag, expFrac};
      ++extendedRange;
      gainTicks >>= 1;
    }
    //Force exact number of bits needed for starting offset not to be fraction of a fraction
    extendedBits = RequiredBits(extendedRange); 
    //Now calculate maximum effective ExpMag, ExpFrac based on current settings at max ExtendedRange
		ticksT effectiveDivisor = 1<<extendedRange;
		ticksT maximumEffectiveTicks = (expMag<<fracBits)+expFrac;
		maximumEffectiveTicks += (((StartingOffset<<magBits)<<fracBits)-1)/effectiveDivisor;
		
    expMag = maximumEffectiveTicks >> fracBits;
    expFrac = FracT(maximumEffectiveTicks & ((ticksT(1u) << ticksT(fracBits)) - ticksT(1)));
		Pair maximumEffective = {expMag,expFrac};
    return ExponentalDecLayout{extendedBits, extendedRange, result, maximumEffective};
  }

  static inline constexpr ExponentalDecLayout Layout = ComputeExponentalDecLayout();
  public:  
    ExponentalDecSelector() noexcept : DefaultPart, CustomPart, DefaultSignedPart() {}
};


template<unsigned magBits, unsigned unsigned fracBits>
class DLL_API ExponentalDec :  RestrictedFloat<ExponentalDecPolicy<magBits, fracBits>>
{
//ExponentalFloat
//64 Bits total size
//6 bits of ExtendedRange
//7 Bits of ExpMag
//25 Bits of ExpFrac
//2 Bits towards signs
//1 Bit towards ZeroSentinal
//+ All ExtraFlags
/* 
Effective (ExpMag, ExpFrac) starting offsets (when not BelowOne)
1: 128, 0 ; Value at 340282366920938463463374607431768211456 (float maximum--the unreachable absolute maximum)
2: 192, 0 (64 ExpMag Gain) Value at 
3: 224, 0 (32 ExpMag Gain)
4: 240, 0 (16 ExpMag Gain)
5: 248, 0 (8 ExpMag Gain)
6: 252, 0 (4 ExpMag Gain) Value at 
7: 254, 0 (2 ExpMag Gain) Value at 
8: 255, 0 (1 ExpMag Gain) Value at 57896044618658097711785492504343953926634992332820282019728792003956564819968
9: 255, 16777216 (255.5 ExpMag) (0.5 ExpMag Gain)
10: (255.75 ExpMag)(8388608 ExpFrac or 0.25 ExpMag  Gain)
11: (4194304 ExpFrac or 0.125 ExpMag Gain)
12: (2097152 ExpFrac or 0.0625 ExpMag Gain)
13: (1048576 ExpFrac or 0.03125 ExpMag Gain)
14: (524288 ExpFrac or 0.015625 ExpMag Gain)
15: (262144 ExpFrac or 0.0078125 ExpMag Gain)
16: (131072 ExpFrac or 0.00390625 ExpMag Gain)
17: (65536 ExpFrac or 0.001953125 ExpMag Gain)
18: (32768 ExpFrac or 0.0009765625 ExpMag Gain)
19: (16384 ExpFrac or 0.00048828125 ExpMag Gain)
20: (8192 ExpFrac or 0.000244140625 ExpMag Gain)
21: (4096 ExpFrac  Gain)
22: (2048 ExpFrac  Gain)
23: (1024 ExpFrac  Gain)
24: (512 ExpFrac  Gain)
25: (256 ExpFrac  Gain)
26: (64 ExpFrac  Gain)
26: (32 ExpFrac  Gain)
27: (16 ExpFrac  Gain)
28: (8 ExpFrac  Gain)
29: (4 ExpFrac  Gain)
30: (2 ExpFrac  Gain)
31: (1 ExpFrac  Gain)--Stop Chart here
32: (0.5 ExpFrac  Gain)
*/

//ExponentalPrecisionFloat
//64 Bits total size
//6 bits of ExtendedRange
//6 Bits of ExpMag
//26 Bits of ExpFrac
//2 Bits towards signs
//1 Bit towards ZeroSentinal
//+ All ExtraFlags
/*
1: 64, 0 Value at 18446744073709551616
2: 96, 0 Value at 9223372036854775808
3: 112, 0 Value at
4: 120, 0 Value at
5: 124, 0 Value at
6: 126, 0 Value at
7: 127, 0 Value at 170141183460469231731687303715884105728 (largest finite float value)
8: 127.5 Value at 240615969168004511545033772477625056927.1149807410631483770082176277113888882495560039681504542806736
9: 127.75 Value at 286142222517866327586855861304318312414.1150395248860346815265983974503994334521390364254268184824545
10: 127.875
*/

//ExponentalStretchedFloat
//64 Bits total size
//6 bits of ExtendedRange
//8 Bits of ExpMag
//24 Bits of ExpFrac
//2 Bits towards signs
//1 Bit towards ZeroSentinal
//+ All ExtraFlags
/* 
Effective (ExpMag, ExpFrac) starting offsets (when not BelowOne)
1: 256, 0 ; Value at 57896044618658097711785492504343953926634992332820282019728792003956564819968 
2: 384, 0 (128 ExpMag Gain) Value at 3.634193621478034452746619039440022671768206803436590301407450995903196440566989616630955253568817828e134
3: 448, 0 (64 ExpMag Gain)
4: 480, 0 (32 ExpMag Gain)
5: 496, 0 (16 ExpMag Gain)
6: 504, 0 (8 ExpMag Gain) Value at 2.618712486316913496010551757462079321773313636834451831586633094476907037123739643906616073860723326e151
7: 508, 0 (4 ExpMag Gain) Value at 4.189939978107061593616882811939326914837301818935122930538612951163051259397983430250585718177157321e152
8: 510, 0 (2 ExpMag Gain) Value at 1.675975991242824637446753124775730765934920727574049172215445180465220503759193372100234287270862928e153
9: 511, 0 (1 ExpMag Gain) Value at 3.351951982485649274893506249551461531869841455148098344430890360930441007518386744200468574541725857e153
*/

//ExponentalDouble
//96 Bits total size
//7 bits of ExtendedRange
//9 Bits of ExpMag
//55 Bits of ExpFrac
//2 Bits towards signs
//1 Bit towards ZeroSentinal
//+ All ExtraFlags
/* Reaches Double ceiling/floor at start of ExtendedRange 7

*/
}