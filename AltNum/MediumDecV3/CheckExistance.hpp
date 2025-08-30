// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <concepts> //C++20 concept

namespace BlazesRusCode
{
  //CheckMemberOrMethodExistance
  struct CheckExistance
  {
      template<class T>
      concept HasValueMemberAndIsAltRepMember =
          requires(T t) {
              { t.Value }    -> std::convertible_to<std::uint32_t>;
              { t.IsAltRep } -> std::convertible_to<bool>;
          };
      
      template<class T>
      concept HasValueMethodAndIsAltRepMethod =
          requires(T t) {
              { t.Value() }    -> std::convertible_to<std::uint32_t>;
              { t.IsAltRep() } -> std::convertible_to<bool>;
          };

      template<class T>
      concept HasFlagsField =
          requires(T t) {
              { t.Flags }    -> std::convertible_to<std::uint32_t>;
          };

      template<class T>
      concept HasValueMember =
          requires(T t) {
              { t.Value }    -> std::convertible_to<std::uint32_t>;
          };
  }
}