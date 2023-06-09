/*	Code Created by James Michael Armstrong (NexusName:BlazesRus)(https://github.com/BlazesRus)
	Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
*/
#pragma once
#if !defined(BufferedVariableArray_IncludeGuard) && !defined(ExcludeExperimentalCode)
#define BufferedVariableArray_IncludeGuard

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

#ifdef BlazesSharedCode_LocalLayout//(Local version style layout)

#else

#endif

//"Unfinished" code here
template <typename VariableType>
class BufferedVariableArray
{public:
	int length=0;
	VariableType* self[];
	BufferedVariableArray()
	{
		self = nullptr;
		length=0;
	}
	//CopyFrom(VariableType* Target){if(self!=nullptr){delete self;}
	//	self = Target;
	//}
	void Delete(){if(self!=nullptr){delete self;self=nullptr;}
		length = 0;
	}
	~BufferedVariableArray()
	{
		if(self!=nullptr){delete self;}
	}
};
#endif
