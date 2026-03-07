// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "VariableConversionFunctions.hpp"

namespace BlazesRusCode
{
  template <typename LaneT, unsigned Limit>
  struct DefaultDecimalPowerTable {
    static constexpr std::array<LaneT, Limit + 1> values =
    VariableConversionFunctions::makePowerOfTens<LaneT, Limit>();
  };
}