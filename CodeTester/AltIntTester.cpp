// ExprFormulaTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>

#include "..\AltNum\AlternativeInt\PartialInt.hpp"
#include "..\AltNum\AlternativeInt\MirroredInt.hpp"
#include "..\AltNum\AlternativeInt\FlaggedInt.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;
using FlaggedInt = BlazesRusCode::FlaggedInt;
#include <Windows.h>

int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);
    MirroredInt test01 = 0;
    PartialInt test02 = 0;
    FlaggedInt test03 = 0;
    //streamObj << LAlt.ToString() <<" + "<< RAlt.ToString() << " = " << AltResult.ToString()<< std::endl;
    ::OutputDebugStringA(streamObj.str().c_str());//Outputing to debug output based on https://www.codeproject.com/Articles/1053/Using-an-Output-Stream-for-Debugging
}
