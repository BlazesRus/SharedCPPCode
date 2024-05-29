// ExprFormulaTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>

#include "..\AltNum\AlternativeInt\PartialInt.hpp"
#include "..\AltNum\AlternativeInt\MirroredInt.hpp"
#include "..\AltNum\AlternativeInt\FlaggedInt.hpp"
#include "..\AltNum\MediumDec\ReducedMediumDecBase.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;
using FlaggedInt = BlazesRusCode::FlaggedInt;
using MediumDec = BlazesRusCode::MediumDec;
#include <Windows.h>//Windows.h detects some errors?

int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);
    MirroredInt test01 = 5;
    PartialInt test02 = 4;
    FlaggedInt test03 = 3;
    test01 *= 10;
    ++test02;
    test03 *= ++test03;
    MediumDec test04.;
    //Need to fix debug output
    //Outputting to debug output based on https://www.codeproject.com/Articles/1053/Using-an-Output-Stream-for-Debugging

    streamObj << "Test01 result:"<<(std::string)test01<< "Test02 result:"<<(std::string)test02<< "Test03 result:"<<(std::string)test03<<std::endl;
    ::OutputDebugStringA(streamObj.str().c_str());
}
