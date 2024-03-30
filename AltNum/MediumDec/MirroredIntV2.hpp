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
#if defined(AltNum_EnableMirroredIntV2)
	struct MirroredIntV2 {
	#pragma options align=bit_packed
	//If value is one then is negative
	unsigned int IsNegative:1;
	//Stores non-signed part of value
	unsigned int Value:31;
	#pragma options align=reset
		MirroredIntV2(unsigned int value=0, unsigned int isNegative=0)
		{
			Value = value;
			IsNegative = isNegative;
		}
	};
#endif
}