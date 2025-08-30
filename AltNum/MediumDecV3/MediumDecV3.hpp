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
	
  struct MediumDecPolicy : FixedDecDefaultPolicy {
      static constexpr bool UnsignedMode = false;
  };

  struct MediumUDecPolicy : FixedDecDefaultPolicy {
      static constexpr bool UnsignedMode = true;
  };

  struct MediumDecWithTrigonomicPolicy : MediumDecPolicy {
      static constexpr bool EnablePi = true;
      static constexpr bool EnableE = true;
      static constexpr bool EnableInfinity = true;
      static constexpr bool EnableApproaching = true;
  };
	
}