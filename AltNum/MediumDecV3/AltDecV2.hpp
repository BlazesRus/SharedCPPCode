// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV3Variant.hpp"

namespace BlazesRusCode
{
  #if !defined(AltNum_UseRepTypeAsClass)
  using RepTypeEnum = BlazesRusCode::RepType;
  #endif

  using MirroredInt = BlazesRusCode::MirroredInt;
  using PartialInt = BlazesRusCode::PartialInt;
  using FlaggedInt = BlazesRusCode::FlaggedInt;
	
  struct AltDecPolicy : MediumDecPolicy {
      // Keep packed storage; turn on ExtraRep for fractional divisor
      static constexpr bool EnableExtraRep          = true;
      using ExtraRepT                                = u32;
      static constexpr bool EnablePi = true;
      static constexpr bool EnableE = true;
  
      static constexpr bool EnablePowerOf           = false;
      static constexpr bool EnableMixedFrac         = false;
  
      // Reasonable default for special states you said you may include
      static constexpr bool EnableInfinity          = true;
      static constexpr bool EnableApproaching       = true;
      static constexpr bool EnableApproachingDivided= true;
      static constexpr bool EnableNaN               = true;
      static constexpr bool EnableWithinMinMaxRange = false;
  
      // If you later add finer toggles like EnableIndeterminateForms or EnableUndefinedInRange,
      // define them here too; your SplitFields can key off them with constexpr.
  };
}