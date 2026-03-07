
#include <iostream>
#include <sstream>
#include <iomanip>

#include <Windows.h>


int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);

    //std::string strTest = (std::string) MediumDectest02;


    ::OutputDebugStringA(streamObj.str().c_str());
}