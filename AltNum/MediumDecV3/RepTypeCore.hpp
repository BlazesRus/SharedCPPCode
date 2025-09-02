// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecCore.hpp"
#include "RepTypeV2.hpp"

namespace BlazesRusCode
{
  template<class VariantName>
  class RepTypeCore : public MediumDecCore<VariantName>
  {
  protected:
    using RepType = BlazesRusCode::RepTypeV2<VariantName>;
  public:
  #pragma region RepType
      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetPiRepType() const
      {
        // Falls through to default if <feature> disabled
        // Sentinal values decided by MediumDecVariantV3
        switch (GetDecimalHalf())
        {
        case VariantName::ApproachingBottomRep:
        if constexpr (VariantName::EnableApproaching)
          return RepType::ApproachingBottomPi; 
        case VariantName::ApproachingTopRep:
        if constexpr (VariantName::EnableApproachingTop)
          return RepType::ApproachingTopPi;
        case VariantName::ApproachingMidLeftRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidLeftPi;
        case VariantName::ApproachingMidRightRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidRightPi;
        default:
          if constexpr (VariantName::EnableExtraRep&&ExtraRep>1){
            if constexpr (VariantName::EnablePowerOf&&ExtraFlags==1)
              return RepType::PiPower;
            else
              return RepType::PiNumByDiv;
          }
          return RepType::PiNum;
        }
      }
      

      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetERepType() const
      {
        // Falls through to default if <feature> disabled
        switch (GetDecimalHalf())
        {
        case VariantName::ApproachingBottomRep:
        if constexpr (VariantName::EnableApproaching)
          return RepType::ApproachingBottomE; 
        case VariantName::ApproachingTopRep:
        if constexpr (VariantName::EnableApproachingTop)
          return RepType::ApproachingTopE;
        case VariantName::ApproachingMidLeftRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidLeftE;
        case VariantName::ApproachingMidRightRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidRightE;
        default:
          if constexpr (VariantName::EnableExtraRep&&ExtraRep>1){
            if constexpr (VariantName::EnablePowerOf&&ExtraFlags==1)
              return RepType::EPower;
            else
              return RepType::ENumByDiv;
          }
          return RepType::ENum;
        }
      }

      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetPiERepType() const
      {
        // Falls through to default if <feature> disabled
        switch (GetDecimalHalf())
        {
        case ApproachingBottomRep:
        if constexpr (VariantName::EnableApproaching)
          return RepType::ApproachingBottomPiE; 
        case ApproachingTopRep:
        if constexpr (VariantName::EnableApproachingTop)
          return RepType::ApproachingTopPiE;
        case ApproachingMidLeftRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidLeftPiE;
        case ApproachingMidRightRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidRightPiE;
        default:
          if constexpr (VariantName::EnableExtraRep&&ExtraRep>1){
            if constexpr (VariantName::EnablePowerOf&&ExtraFlags==1)
              return RepType::PiEPower;
            else
              return RepType::PiENumByDiv;
          }
          return RepType::PiENum;
        }
      }

      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetIRepType() const
      {
        // Falls through to default if <feature> disabled
        switch (GetDecimalHalf())
        {
        case VariantName::ApproachingBottomRep:
        if constexpr (VariantName::EnableApproaching)
          return RepType::ApproachingBottomI; 
        case VariantName::ApproachingTopRep:
        if constexpr (VariantName::EnableApproachingTop)
          return RepType::ApproachingTopI;
        case VariantName::ApproachingMidLeftRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidLeftI;
        case VariantName::ApproachingMidRightRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidRightI;
        case VariantName::InfinityRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ImaginaryInfinity;
        default:
          if constexpr (VariantName::EnableExtraRep&&ExtraRep>1){
            if constexpr (VariantName::EnableMixedFrac&&ExtraFlags==1)
              return RepType::MixedI;
            else
              return RepType::INumByDiv;
          }
          return RepType::INum;
        }
      }

      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetNormRepType() const
      {
        switch (GetDecimalHalf())
        {
        case VariantName::ApproachingBottomRep:
        if constexpr (VariantName::EnableApproaching)
          return RepType::ApproachingBottom; 
        case VariantName::ApproachingTopRep:
        if constexpr (VariantName::EnableApproachingTop)
          return RepType::ApproachingTop;
        case VariantName::ApproachingMidLeftRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidLeft;
        case VariantName::ApproachingMidRightRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::ApproachingMidRight;
        case VariantName::InfinityRep:
        if constexpr (VariantName::EnableApproachingDivided)
          return RepType::Infinity;
        default:
          if constexpr (VariantName::EnableExtraRep&&ExtraRep>1){
            if constexpr ((VariantName::EnablePowerOf||VariantName::EnableMixedFrac)&&ExtraFlags==1)
              return constexpr (VariantName::EnablePowerOf)?RepType::ToPowerOf:;
            else
              return RepType::INumByDiv;
          }
          return RepType::INum;
        }
      }


      /// <summary>
      /// Returns representation type data that is stored in value
      /// </summary>
      RepType GetRepType() const;

    #pragma endregion RepType
  };
}
