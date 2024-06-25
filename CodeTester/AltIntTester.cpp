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
#include <Windows.h>//Windows.h detects some errors?

int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);
    MirroredInt test01 = 5;
    MirroredInt test02 = 4;
    test02 = test01;
    MirroredInt test03 = test01;
    PartialInt Partialtest02 = 4;
    PartialInt Partialtest03 = Partialtest02;
    FlaggedInt Flaggedtest03 = FlaggedInt::Zero;
    FlaggedInt Flaggedtest04 = Flaggedtest03;
    test01 *= 10;
    test02 /= -2;
    ++test02;
    test03 *= ++test03;
    Partialtest02 += 5;
    Flaggedtest03 /= 2;
    std::string strTest = (std::string) test01;
    std::string strTest02 = (std::string) Partialtest02;
    std::string strTest03 = (std::string) Flaggedtest03;
    //Need to fix debug output
    //Outputting to debug output based on https://www.codeproject.com/Articles/1053/Using-an-Output-Stream-for-Debugging

    //streamObj << "Test01 result:"<<(std::string)test01<< "Test02 result:"<<(std::string)test02<< "Test03 result:"<<(std::string)test03<<std::endl;
    ::OutputDebugStringA(streamObj.str().c_str());
}
