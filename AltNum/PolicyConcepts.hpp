#pragma once

#include <concepts>

//BlazesRusPolicyConcepts portion of BlazesRusCode
namespace BlazesRusCode
{
	template<typename Policy>
  concept Has_UnsignedMode = requires { { Policy::UnsignedMode } -> std::convertible_to<bool>; };
	template<typename Policy>
  concept Has_SignedMode   = requires { { Policy::SignedMode   } -> std::convertible_to<bool>; };
	
	template<typename Policy>
	concept Has_PositiveSign =
			requires {
					{ Policy::PositiveSign } -> std::integral;
			} &&
			((Policy::PositiveSign == 0) || (Policy::PositiveSign == 1));
	
	template<typename Policy>
	concept Has_NegativeSign =
			requires {
					{ Policy::NegativeSign } -> std::integral;
			} &&
			((Policy::NegativeSign == 0) || (Policy::NegativeSign == 1));
	
	#pragma region RestrictedFloat Variant Specific
	template<typename Policy>
  concept Has_EXPMAG_BITS   = requires { { Policy::EXPMAG_BITS }   -> std::convertible_to<unsigned>; };
	template<typename Policy>
  concept Has_EXPFRAC_BITS   = requires { { Policy::EXPFRAC_BITS }   -> std::convertible_to<unsigned>; };
	template<typename Policy>
  concept Has_NScaleFactor   = requires { { Policy::NScaleFactor }   -> std::convertible_to<unsigned>; };

  template<typename Policy>
  concept Has_ZeroExpRep = requires { { Policy::ZeroExpRep } -> std::convertible_to<unsigned>; };
	template<typename Policy>
  concept Has_ZeroExpFracRep   = requires { { Policy::ZeroExpFracRep }   -> std::convertible_to<unsigned>; };
	#pragma endregion RestrictedFloat Variant Specific
	
	#pragma region RestrictedFloat Standalone Specific
	template<typename Policy>
  concept Has_StandaloneMode   = requires { { Policy::StandaloneMode }   -> std::convertible_to<bool>; };
	
	template<typename Policy>
  concept Has_SignedExpMode   = requires { { Policy::SignedExpMode }   -> std::convertible_to<bool>; };
	
	#pragma endregion RestrictedFloat Standalone Specific
	
	#pragma region RestrictedFloat CustomDenom Specific
	template<typename Policy>
  concept Has_UseCustomDenom    = requires { { Policy::UseCustomDenom }   -> std::convertible_to<bool>; };
	
	#pragma endregion RestrictedFloat CustomDenom Specific
	
	#pragma region MediumDec Variant Specific
	template<typename Policy>
	concept Has_EnablePi   = requires { { Policy::EnablePi }   -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableE    = requires { { Policy::EnableE }    -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnablePiE  = requires { { Policy::EnablePiE }  -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableI    = requires { { Policy::EnableI }    -> std::convertible_to<bool>; };
	
	template<typename Policy>
	concept Has_EnableInfinity        = requires { { Policy::EnableInfinity }        -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableApproaching     = requires { { Policy::EnableApproaching }     -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_DisableApproachingTop = requires { { Policy::DisableApproachingTop } -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableNaN             = requires { { Policy::EnableNaN }             -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableApproachingDivided = requires { { Policy::EnableApproachingDivided } -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableIndeterminateForms = requires { { Policy::EnableIndeterminateForms } -> std::convertible_to<bool>; };
	
	template<typename Policy>
	concept Has_SplitFieldsMode = requires { { Policy::SplitFieldsMode } -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_BitwiseMaskMode = requires { { Policy::BitwiseMaskMode } -> std::convertible_to<bool>; };
	
	template<typename Policy>
	concept Has_EnableExtraRep         = requires { { Policy::EnableExtraRep }         -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableMaskedExtraRep   = requires { { Policy::EnableMaskedExtraRep }   -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableMixedFrac        = requires { { Policy::EnableMixedFrac }        -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnablePowerOf          = requires { { Policy::EnablePowerOf }          -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableWithinMinMaxRange    = requires { { Policy::EnableWithinMinMaxRange } -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableNotWithinMinMaxRange = requires { { Policy::EnableNotWithinMinMaxRange } -> std::convertible_to<bool>; };
	
	template<typename Policy>
	concept Has_MaxIntHalfMag   = requires { { Policy::MaxIntHalfMag }   -> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_IntValue_BITS   = requires { { Policy::IntValue_BITS }   -> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_DecimalHalf_BITS= requires { { Policy::DecimalHalf_BITS }-> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_FlagState_BITS  = requires { { Policy::FlagState_BITS }  -> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_MaskedExtra_BITS= requires { { Policy::MaskedExtra_BITS }-> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_ExtraFlag_BITS  = requires { { Policy::ExtraFlag_BITS }  -> std::convertible_to<unsigned>; };
	template<typename Policy>
	concept Has_MixedFracDivisorLimit = requires { { Policy::MixedFracDivisorLimit } -> std::convertible_to<unsigned>; };
	
	template<typename Policy>
	concept Has_BinaryFractionMode      = requires { { Policy::BinaryFractionMode }      -> std::convertible_to<bool>; };
	template<typename Policy>
	concept Has_EnableOverflowProtection= requires { { Policy::EnableOverflowProtection }-> std::convertible_to<bool>; };

	template<typename Policy>
	concept Has_SkipSafeCheck = requires { { Policy::Has_SkipSafeCheck }-> std::convertible_to<bool>; };
	#pragma endregion MediumDec Variant Specific
}