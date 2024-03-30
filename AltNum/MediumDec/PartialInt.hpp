// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#ifdef BlazesSharedCode_LocalLayout
#ifndef DLL_API
#ifdef UsingBlazesSharedCodeDLL
#define DLL_API __declspec(dllimport)
#elif defined(BLAZESSharedCode_LIBRARY)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif
#endif
#else
#include "..\DLLAPI.h"
#endif

namespace BlazesRusCode
{
#if !defined(AltNum_UseIntForDecimalHalf)
	struct PartialInt {
	#pragma options align=bit_packed
	//Stores Digits XXX XXX XXX
	unsigned int Value:30;
	//Can store up to 4 Flag states including normal state at 0
	unsigned int Flags:2;
	#pragma options align=reset
		PartialInt(unsigned int value=0, unsigned int flags=0)
		{
			Value = value;
			Flags = flags;
		}
	};
#endif
}