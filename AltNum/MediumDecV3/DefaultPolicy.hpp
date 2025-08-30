// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <cstdint>
#include <limits>
#include <bit>      // C++20 std::bit_width
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

  //Default Sentinal Policy for MediumDecV3
  struct FixedDecDefaultPolicy {
    // Compile-time knobs

    static constexpr bool PositiveSign = true; // 1 means positive
    static constexpr bool UnsignedMode = true;
    using StoreT      = u64;
    
    //EnablePi, EnableE, or EnableI required for EnableStateFlags to be set
    //static constexpr bool EnableStateFlags = false;
    static constexpr bool EnablePi = false;
    static constexpr bool EnableE = false;
    static constexpr bool EnableI = false;
    
    static constexpr bool EnableInfinity = false;
    static constexpr bool EnableApproaching = false;
    static constexpr bool DisableApproachingTop = false;
    static constexpr bool EnableNaN = false;
    static constexpr bool EnableApproachingDivided = false;
		//Defined when ExtraRep is equal to 0, Represents a number within bounds of (Sign*IntHalf.Value) to (ExtraRep.IsAltRep?-1:1)*(DecimalHalf.Value)
    static constexpr bool EnableWithinMinMaxRange = false;
		//Defined when ExtraRep is equal to ??????????, Represents a number not within bounds of (Sign*IntHalf.Value) to (ExtraRep.IsAltRep?-1:1)*(DecimalHalf.Value)
    static constexpr bool EnableNotWithinMinMaxRange = false;
    static constexpr bool EnableIndeterminateForms = false; 
    
    //Use either FlaggedInt or u32 for ExtraRep (FlaggedInt required if use either EnablePowerOf or EnableMixedFrac)
    static constexpr bool EnableExtraRep = false;
    //If EnableMaskedExtraRep is enabled, than ExtraRep used raw lane to store values(because uint96 does not exist)
    static constexpr bool EnableMaskedExtraRep = false;
    //ExtraRep only exists as variable if EnableExtraRep is enabled
    using ExtraRepT      = u32;
    //If EnablePowerOf or EnableMixedFrac is enabled, then reinterprets DecimalHalf mask if ExtraRep.IsAltRep is true
    static constexpr bool EnablePowerOf = false;
    static constexpr bool EnableMixedFrac = false;
    //If IsExtraRepUsingBitMasks is set then, Value() and IsAltRep() are functions in ExtraRepT instead of member variables
    static constexpr bool IsExtraRepUsingBitMasks = false;

    static constexpr unsigned IntHalfBits = 32;
    static constexpr unsigned DecimalHalfBits = 30;
    static constexpr unsigned ValueBits = 62;
    //Pi,e,i state flags
    static constexpr unsigned stateFlagBits = 2;

    static constexpr unsigned MaskedExtraBits = 0;
    
    //If enabled OpMethods return Trailing digits lost during operation for use in MixedDec variant operations
    static constexpr bool EnableMixedMode = false;
    
    //If DisableBitwiseMaskMode is enabled it will use 
    static constexpr bool DisableBitwiseMaskMode = false;
    //Defines default type for IntHalf portion
    using IntHalfT      = u32;
    using IntHalfValueT      = u32;
    //Defines default type for DecimalHalf portion
    using DecimalHalfT      = u32;
    using DecimalHalfValueT      = u32;
    
    static constexpr unsigned FractionalMaximum = 2147483647;
    static constexpr unsigned MixedFracDivisorLimit = 1073741804;//InfinityRep-1

    static constexpr unsigned MaxIntHalf = 4294967295; 
    static constexpr unsigned MinIntHalf = 0;

    //template <typename LaneT, unsigned Limit>
    //struct DefaultDecimalPowerTable {
    //    static constexpr std::array<LaneT, Limit + 1> values =
    //        VariableConversionFunctions::makePowerOfTens<LaneT, Limit>();
    //};

    //Default Decimal max digits
    static constexpr DecimalHalfDigits = 9;

    static constexpr IntHalfT IntHalfZero = 0;
    static constexpr DecimalHalfT DecimalHalfZero = 0;
    static constexpr IntHalfT IntHalfOne = 1;
    static constexpr DecimalHalfT DecimalHalfOne = 1;
    static constexpr IntHalfT IntHalfNegOne = -1;
    static constexpr DecimalHalfT DecimalHalfNegOne = -1;
    static constexpr ExtraRepHalfT DecimalHalfZero = 0;

    //static_assert(Policy::DecimalHalfDigits + 1 < std::size(VariableConversionFunctions::PowerOfTens64Bit),
    //              "DecimalHalfDigits out of range for PowerOfTens64Bit");
  };

  template<typename P>
  concept FixedDecPolicyVariant =
    // Type aliases
    requires { typename P::StoreT; } &&
  
    // True constexpr flags
    (std::same_as<decltype(P::UnsignedMode), const bool> &&
     std::same_as<decltype(P::EnablePi), const bool> &&
     std::same_as<decltype(P::EnableE), const bool> &&
     std::same_as<decltype(P::EnableI), const bool> &&
     std::same_as<decltype(P::EnableExtraRep), const bool> &&
     std::same_as<decltype(P::EnableApproaching), const bool> &&
     std::same_as<decltype(P::EnableInfinity),   const bool>) &&
  
    // Integral constants
    (std::same_as<decltype(P::IntHalfBits),      const unsigned> &&
     std::same_as<decltype(P::DecimalHalfBits),    const unsigned> &&
     std::same_as<decltype(P::ValueBits),    const unsigned> &&
     std::same_as<decltype(P::stateFlagBits),const unsigned>);
}