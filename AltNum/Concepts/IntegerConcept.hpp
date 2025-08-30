#pragma once

namespace BlazesRusCode
{
    //Integer type (concept)
    template<typename T>
    concept IntegerType = std::is_integral_v<T>;
		
    //Unsigned Integer type (concept)
    template<typename T>
    concept UnsignedIntegerType = std::is_unsigned_v<T>&&std::is_integral_v<T>;
		
    //Signed Integer type (concept)
    template<typename T>
    concept SignedIntegerType = std::is_signed_v<T>&&std::is_integral_v<T>;
}