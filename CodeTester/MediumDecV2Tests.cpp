
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

#include "..\AltNum\MediumDec\MediumDec.hpp"
using MediumDec = BlazesRusCode::MediumDec;

//#include "..\AltNum\MediumDecV2\MediumDecV2.hpp"
//using MediumDecV2 = BlazesRusCode::MediumDecV2;

int main()
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(99);
    MediumDec MediumDectest02 = 4;
    MediumDec MediumDectest03 = MediumDectest02;

    MediumDectest02 *= 10;
    MediumDectest02 /= -2;
    MediumDectest02 += 5;

    std::string strTest = (std::string) MediumDectest02;
/*
    MediumDecV2 MediumDecV2test03 = MediumDecV2::Zero;
    MediumDecV2 MediumDecV2test04 = MediumDecV2test03;
    ++MediumDecV2test03;
    MediumDecV2test04 *= ++MediumDecV2test03;
    MediumDecV2test03 /= 2;
    std::string strTest02 = (std::string) MediumDecV2test03;
*/


    ::OutputDebugStringA(streamObj.str().c_str());
}