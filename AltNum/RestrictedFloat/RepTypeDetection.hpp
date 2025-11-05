// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <cstdint>
#include <concepts>

namespace RepTypeDetection {
//If policy exists and is true
#pragma region Policy ExistanceToggles
	template<typename Policy>
	concept Has_EnablePi   = requires { { Policy::EnablePi }   -> std::same_as<bool>; }&&(Policy::EnablePi);
	template<typename Policy>
	concept Has_EnableE    = requires { { Policy::EnableE }    -> std::same_as<bool>; }&&(Policy::EnableE);
	template<typename Policy>
	concept Has_EnableI    = requires { { Policy::EnableI }    -> std::same_as<bool>; }&&(Policy::EnableI);
  template<typename Policy>
  concept Has_EnablePiE = requires {
    { Policy::EnablePi } -> std::same_as<bool>;
    { Policy::EnableE }  -> std::same_as<bool>;
  }&&(Policy::EnablePi&&Policy::EnableE);
  template<typename Policy>
  concept Has_EnableEI = requires {
    { Policy::EnableE }  -> std::same_as<bool>;
    { Policy::EnableI }    -> std::same_as<bool>;
  }&&(Policy::EnableE&&Policy::EnableI);
  template<typename Policy>
  concept Has_EnablePiEI = requires {
    { Policy::EnablePi } -> std::same_as<bool>;
    { Policy::EnableE }  -> std::same_as<bool>;
    { Policy::EnableI }    -> std::same_as<bool>;
  }&&(Policy::EnablePi&&Policy::EnableE&&Policy::EnableI);

	template<typename Policy>
	concept Has_EnablePhi    = requires { { Policy::EnablePhi }    -> std::same_as<bool>; }&&(Policy::EnablePhi);
	template<typename Policy>
	concept Has_EnableLnXNum    = requires { { Policy::EnableLnXNum }    -> std::same_as<bool>; }&&(Policy::EnableLnXNum);
	template<typename Policy>
	concept Has_EnablePlasticNum    = requires { { Policy::EnablePlasticNum }    -> std::same_as<bool>; }&&(Policy::EnablePlasticNum);

	template<typename Policy>
	concept Has_EnableIndeterminateForms    = requires { { Policy::EnableIndeterminateForms }-> std::same_as<bool>; }&&(Policy::EnableIndeterminateForms);

	template<typename Policy>
	concept Has_EnableFractionals    = requires { { Policy::EnableFractionals }-> std::same_as<bool>; }&&(Policy::EnableFractionals);

	template<typename Policy>
	concept Has_EnableReciprocalOf    = requires { { Policy::EnableReciprocalOf }-> std::same_as<bool>; }&&(Policy::EnableReciprocalOf);

	template<typename Policy>
	concept Has_EnableInversedOf    = requires { { Policy::EnableInversedOf }    -> std::same_as<bool>; }&&(Policy::EnableInversedOf);

	template<typename Policy>
	concept Has_EnableApproaching    = requires { { Policy::EnableApproaching }    -> std::same_as<bool>; }&&(Policy::EnableApproaching);
	
	template<typename Policy>
	concept Has_SignedZero    = requires { { Policy::EnableSignedZero }    -> std::same_as<bool>; }&&(Policy::EnableSignedZero);
	
	template<typename Policy>
	concept Has_EnableApproachingTop    = requires { { Policy::EnableApproachingTop }    -> std::same_as<bool>; }&&(Policy::EnableApproachingTop);
	template<typename Policy>
	concept Has_EnableApproachingDivided    = requires {
    { Policy::EnableFractionals }    -> std::same_as<bool>;
    { Policy::EnableApproaching }    -> std::same_as<bool>;
    { Policy::EnableApproachingDivided }    -> std::same_as<bool>;
  }&&(Policy::EnableFractionals&&Policy::EnableApproaching&&Policy::EnableApproachingDivided);
	template<typename Policy>
	concept Has_EnableApproachingOffsets    = requires {
    { Policy::EnableApproaching }    -> std::same_as<bool>;
    { Policy::EnableApproachingOffsets }    -> std::same_as<bool>;
  }&&(Policy::EnableApproaching&&Policy::EnableApproachingOffsets);

	template<typename Policy>
	concept Has_EnableTrigExpressions    = requires { { Policy::EnableTrigExpressions }-> std::same_as<bool>; }&&(Policy::EnableTrigExpressions);

	template<typename Policy>
	concept Has_EnableHyperbolicExpressions    = requires { { Policy::EnableHyperbolicExpressions }  -> std::same_as<bool>; }&&(Policy::EnableHyperbolicExpressions);

	template<typename Policy>
	concept Has_EnableInfinity    = requires { { Policy::EnableInfinity }-> std::same_as<bool>; }&&(Policy::EnableInfinity);

	template<typename Policy>
	concept Has_EnablePowerOf    = requires { { Policy::EnablePowerOf }    -> std::same_as<bool>; }&&(Policy::EnablePowerOf);
	
	template<typename Policy>
	concept Has_EnableExtraRepMultiplier    = requires { { Policy::EnableExtraRepMultiplier }    -> std::same_as<bool>; }&&(Policy::EnableExtraRepMultiplier);

	template<typename Policy>
	concept Has_EnableMixedFrac    = requires { { Policy::EnableMixedFrac }    -> std::same_as<bool>; }&&(Policy::EnableMixedFrac);

	template<typename Policy>
	concept Has_EnableWithinMinMaxRange    = requires { { Policy::EnableWithinMinMaxRange }    -> std::same_as<bool>; }&&(Policy::EnableWithinMinMaxRange);
	template<typename Policy>
	concept Has_EnableNotWithinMinMaxRange    = requires { { Policy::EnableNotWithinMinMaxRange }    -> std::same_as<bool>; }&&(Policy::ExtraRepBits);

	template<typename Policy>
	concept Has_ExtraRepBits  = requires { { Policy::ExtraRepBits }  -> std::unsigned_integral; }&&(Policy::ExtraRepBits>0);

	template<typename Policy>
	concept Has_EnableComplexNumbers    = requires { { Policy::EnableComplexNumbers }-> std::same_as<bool>; }&&(Policy::EnableComplexNumbers);

	template<typename Policy>
	concept SignedExpEnabled    = requires { { Policy::SignedExpMode }    -> std::same_as<bool>; }&&(Policy::SignedExpMode);

	template<typename Policy>
	concept SignEnabled    = requires { { Policy::SignedMode }    -> std::same_as<bool>; }&&(Policy::SignedMode);

#pragma endregion Policy ExistanceToggles
#pragma region Layout Toggles
	template<typename Layout>
	concept PiRepresented   = requires { { Layout::PiNumRep }   -> std::unsigned_integral; }&&(Layout::PiNumRep!=0);
	template<typename Layout>
	concept ERepresented    = requires { { Layout::ENumRep }    -> std::unsigned_integral; }&&(Layout::ENumRep!=0);
	template<typename Layout>
	concept IRepresented    = requires { { Layout::INumRep }    -> std::unsigned_integral; }&&(Layout::INumRep!=0);
  template<typename Layout>
  concept PiERepresented = requires {
    { Layout::PiNumRep } -> std::unsigned_integral;
    { Layout::ENumRep }  -> std::unsigned_integral;
  }&&(Layout::PiNumRep!=0&&Layout::ENumRep!=0);
  template<typename Layout>
  concept EIRepresented = requires {
    { Layout::ENumRep } -> std::unsigned_integral;
    { Layout::INumRep } -> std::unsigned_integral;
  }&&(Layout::ENumRep!=0&&Layout::INumRep!=0);
  template<typename Layout>
  concept PiEIRepresented = requires {
    { Layout::PiNumRep } -> std::unsigned_integral;
    { Layout::ENumRep }  -> std::unsigned_integral;
    { Layout::INumRep }  -> std::unsigned_integral;
  }&&(Layout::PiNumRep!=0&&Layout::ENumRep!=0&&Layout::INumRep!=0);

	template<typename Layout>
	concept PhiRepresented    = requires { { Layout::PhiNumRep }    -> std::unsigned_integral; }&&(Layout::PhiNumRep!=0);
	template<typename Layout>
	concept LnXNumRepresented    = requires { { Layout::LnXNumRep }    -> std::unsigned_integral; }&&(Layout::LnXNumRep!=0);
	template<typename Layout>
	concept PlasticNumRepresented    = requires { { Layout::PlasticNumRep }    -> std::unsigned_integral; }&&(Layout::PlasticNumRep!=0);

	template<typename Layout>
	concept IndeterminateFormsRepresented    = requires { { Layout::IndeterminateForms }-> std::unsigned_integral; }&&(Layout::IndeterminateForms!=0);

	template<typename Layout>
	concept ByDivRepresented    = requires { { Layout::NumByDivRep }-> std::unsigned_integral; }&&(Layout::NumByDivRep!=0);

  //Divided by binary shifts
	template<typename Layout>
	concept ByBinaryDivRepresented    = requires { { Layout::NumByBinaryDivRep }-> std::unsigned_integral; }&&(Layout::NumByBinaryDivRep!=0);

	template<typename Layout>
	concept ReciprocalRepresented    = requires { { Layout::ReciprocalOfRep }-> std::unsigned_integral; }&&(Layout::ReciprocalOfRep!=0);

	template<typename Layout>
	concept InversionRepresented    = requires { { Layout::InversedOfRep }    -> std::unsigned_integral; }&&(Layout::InversedOfRep!=0);

	template<typename Layout>
	concept ApproachingRepresented    = requires { { Layout::ApproachingBottomRep }    -> std::unsigned_integral; }&&(Layout::ApproachingBottomRep!=0);
	template<typename Layout>
	concept ApproachingTopRepresented    = requires { { Layout::ApproachingTopRep }    -> std::unsigned_integral; }&&(Layout::ApproachingTopRep!=0);
	
	template<typename Layout>
	concept ApproachingBottomByDivRepresented    = requires {
    { Layout::ApproachingBottomRep }    -> std::unsigned_integral;
    { Layout::NumByDivRep }-> std::unsigned_integral;
  }&&(Layout::ApproachingBottomRep!=0&&Layout::NumByDivRep!=0);
	template<typename Layout>
	concept ApproachingTopByDivRepresented    = requires {
    { Layout::ApproachingTopRep }    -> std::unsigned_integral;
    { Layout::NumByDivRep }-> std::unsigned_integral;
  }&&(Layout::ApproachingBottomRep!=0&&Layout::NumByDivRep!=0);
	
	template<typename Layout>
	concept ApproachingMidRepresented    = requires {
    { Layout::ApproachingBottomRep }    -> std::unsigned_integral;
    { Layout::ApproachingTopRep }    -> std::unsigned_integral;
    { Layout::ApproachingMidLeftRep }    -> std::unsigned_integral;
    { Layout::ApproachingMidRightRep }    -> std::unsigned_integral;
  }&&(Layout::ApproachingBottomRep!=0&&Layout::ApproachingTopRep!=0
  &&Layout::ApproachingMidLeftRep!=0&&Layout::ApproachingMidRightRep!=0);
	template<typename Layout>
	concept ApproachingOffsetsRepresented    = requires {
    { Layout::ApproachingBottomRep }    -> std::unsigned_integral;
    { Layout::ApproachingTopRep }    -> std::unsigned_integral;
    { Layout::PlusOffsetRep }    -> std::unsigned_integral;
    { Layout::MinusOffsetRep }    -> std::unsigned_integral;
  }&&(Layout::ApproachingBottomRep!=0&&Layout::ApproachingTopRep!=0
  &&Layout::PlusOffsetRep!=0&&Layout::MinusOffsetRep!=0);

	template<typename Layout>
  concept TrigRepresented = requires {
    { Layout::SinRep } -> std::unsigned_integral;
    { Layout::CosRep }  -> std::unsigned_integral;
    { Layout::TanRep }  -> std::unsigned_integral;
  }&&(Layout::SinRep!=0&&Layout::CosRep!=0&&Layout::TanRep);
	template<typename Layout>
  concept HyperbolicRepresented = requires {
    { Layout::SinHRep } -> std::unsigned_integral;
    { Layout::CosHRep }  -> std::unsigned_integral;
    { Layout::TanHRep }  -> std::unsigned_integral;
  }&&(Layout::SinHRep!=0&&Layout::CosHRep!=0&&Layout::TanHRep);
	
	template<typename Layout>
	concept InfinityRepresented    = requires { { Layout::InfinityRep }-> std::unsigned_integral; }&&(Layout::InfinityRep!=0);
	template<typename Layout>
	concept UniverseUnitsRepresented    = requires { { Layout::UniverseUnitsRep }-> std::unsigned_integral; }&&(Layout::UniverseUnitsRep!=0);

	template<typename Layout>
	concept PowerOfRepresented    = requires { { Layout::ToPowerOfRep }    -> std::unsigned_integral; }&&(Layout::ToPowerOfRep!=0);
	
	template<typename Layout>
	concept SqrtOfRepresented    = requires { { Layout::SqrtOfRep }    -> std::unsigned_integral; }&&(Layout::SqrtOfRep!=0);
		
	template<typename Layout>
	concept ExtraPiRepresented    = requires {
    { Layout::PiNumExtraRep }   -> std::unsigned_integral;
  }&&(Layout::PiNumExtraRep!=0);
	template<typename Layout>
	concept ExtraERepresented    = requires {
    { Layout::ENumExtraRep }    -> std::unsigned_integral;
  }&&(Layout::ENumExtraRep!=0);
	template<typename Layout>
	concept ExtraIRepresented    = requires {
    { Layout::INumExtraRep }    -> std::unsigned_integral;
  }&&(Layout::INumExtraRep!=0);
	template<typename Layout>
	concept ExtraPiERepresented    = requires {
    { Layout::PiENumExtraRep }  -> std::unsigned_integral;
  }&&(Layout::PiENumExtraRep!=0);
	template<typename Layout>
	concept ExtraPiIRepresented    = requires {
    { Layout::PiINumExtraRep }    -> std::unsigned_integral;
  }&&(Layout::PiINumExtraRep!=0);
	template<typename Layout>
	concept ExtraPiEIRepresented    = requires {
    { Layout::PiEINumExtraRep }    -> std::unsigned_integral;
  }&&(Layout::PiEINumExtraRep!=0);

	template<typename Layout>
	concept MixedFracRepresented    = requires { { Layout::MixedFracRep }    -> std::unsigned_integral; }&&(Layout::MixedFracRep!=0);

	template<typename Layout>
	concept WithinMinMaxRangeRepresented    = requires { { Layout::WithinMinMaxRangeRep }    -> std::unsigned_integral; }&&(Layout::WithinMinMaxRangeRep!=0);
	template<typename Layout>
	concept NotWithinMinMaxRangeRepresented    = requires { { Layout::NotWithinMinMaxRangeRep }    -> std::unsigned_integral; }&&(Layout::ExtraRepBits!=0);
#pragma endregion Layout Toggles

  template<class Layout>
  using FullRepUnderlying = BinaryTypeSelector::TypeForBits<Layout::totalBits>;
	
  template<class Layout>
  using RepUnderlying = BinaryTypeSelector::TypeForBits<Layout::totalWithMode>;
}