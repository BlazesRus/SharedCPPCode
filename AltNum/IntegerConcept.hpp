namespace BlazesRusCode
{
    //Integer type (concept)
    template<typename T>
    concept IntegerType = std::is_integral<T>::value;
}