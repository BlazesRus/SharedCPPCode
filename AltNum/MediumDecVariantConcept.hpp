#pragma once

#include <type_traits>
#include "..\MediumDec\AltNumBase.h"

namespace BlazesRusCode
{//https://en.cppreference.com/w/cpp/concepts/derived_from
    //MediumDec Variant (concept)
	//derived from info at https://en.cppreference.com/w/cpp/concepts/derived_from
	//is base of info at https://en.cppreference.com/w/cpp/types/is_base_of
	template<typename T>
    concept MediumDecVariant = std::is_base_of_v<AltNumBase, T>::value;
}