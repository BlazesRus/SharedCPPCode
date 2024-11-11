#pragma once

#include "..\MediumDec\AltNumBase.h"

#include <concepts>
#include <type_traits>

namespace BlazesRusCode
{//https://en.cppreference.com/w/cpp/concepts/derived_from

    //Base class for AltDec and SmallDec identify class as SmallDec variant
    class DLL_API SmallNumBase
    {
    
    };
    
    //Base class for AltDec and SmallUDec identify class as SmallUDec variant
    class DLL_API SmallUNumBase
    {
    
    };

    /// <summary>
    /// SmallDec Variant (concept)
    /// derived from info at https://en.cppreference.com/w/cpp/concepts/derived_from
    /// is base of info at https://en.cppreference.com/w/cpp/types/is_base_of
    /// </summary>
    template<typename T>
    concept SmallDecVariant = std::is_base_of<SmallNumBase, T>::value;
    
    /// <summary>
    /// SmallDec unsigned Variant (concept)
    /// derived from info at https://en.cppreference.com/w/cpp/concepts/derived_from
    /// is base of info at https://en.cppreference.com/w/cpp/types/is_base_of
    /// </summary>
    template<typename T>
    concept SmallUDecVariant = std::is_base_of<SmallUNumBase, T>::value;
}