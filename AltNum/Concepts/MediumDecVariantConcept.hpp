#pragma once

#include "..\MediumDec\AltNumBase.h"

#include <concepts>
#include <type_traits>

namespace BlazesRusCode
{//https://en.cppreference.com/w/cpp/concepts/derived_from
    //MediumDec Variant (concept)
	//derived from info at https://en.cppreference.com/w/cpp/concepts/derived_from
	//is base of info at https://en.cppreference.com/w/cpp/types/is_base_of
	template<typename T>
    concept MediumDecVariant = std::is_base_of<AltNumBase, T>::value;
	
    //unsigned MediumDec Variant (concept)
	//derived from info at https://en.cppreference.com/w/cpp/concepts/derived_from
	//is base of info at https://en.cppreference.com/w/cpp/types/is_base_of
	template<typename T>
    concept MediumUDecVariant = std::is_base_of<UnsignedAltNumBase, T>::value;
}