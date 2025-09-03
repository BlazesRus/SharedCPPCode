// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

namespace BlazesRusCode
{
  struct NoExtraRep {
	};

	template<typename Policy>
	struct ExtraRepFields
	{
		ExtraRepT ExtraRep = 1;
		
		constexpr void EnforceExtraRepShapeIfAltRep()
		{
			if constexpr (Policy::EnableExtraRep&&Policy::EnableExtraRepFlags) {
				static_assert(
						CheckMemberOrMethodExistance::HasValueMemberAndIsAltRepMember<typename Policy::ExtraRepT>,
						"In Non-Masked ExtraRep mode, ExtraRepT must have Value and IsAltRep members if using ExtraFlags."
				);
			}
		}

		EnforceExtraRepShapeIfAltRep();

		// ctor / init to zero or whatever you like
		ExtraRepFields() noexcept : ExtraRep{} {}
	
}