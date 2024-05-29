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
	#include "..\..\DLLAPI.h"
#endif

namespace BlazesRusCode
{
	//Base class for AltDec and MediumDec identify class as MediumDec variant
    virtual class DLL_API AltNumBase
    {
	
    };
}