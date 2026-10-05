
#include <iostream>
#include <sstream>
#include <iomanip>

#include <Windows.h>
#include <string>
#include <cmath>

#include <type_traits>
#include <cstddef>
#include <bit>//used for C++20 std::bit_cast
#include <compare>//used for C++20 feature of spaceship operator
#include <cstdint>


int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);

    //std::string strTest = (std::string) MediumDectest02;

    uint64_t Raw = 4082400000;

    uint64_t TopBitIndex = std::bit_width(Raw);
    std::string strTest = std::to_string(TopBitIndex);
    printf(strTest.c_str());

    //::OutputDebugStringA(streamObj.str().c_str());
}