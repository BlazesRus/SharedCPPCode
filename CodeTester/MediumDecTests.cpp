// ExprFormulaTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>

#include "..\MediumDec\MediumDec.hpp"
using MediumDecVariant = BlazesRusCode::MediumDec;

#include <Windows.h>

int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);
    //streamObj << LAlt.ToString() <<" + "<< RAlt.ToString() << " = " << AltResult.ToString()<< std::endl;
    ::OutputDebugStringA(streamObj.str().c_str());//Outputing to debug output based on https://www.codeproject.com/Articles/1053/Using-an-Output-Stream-for-Debugging
}
