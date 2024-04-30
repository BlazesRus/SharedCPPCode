// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "RepType.h"

namespace BlazesRusCode
{
//Forward calling virtual table based on https://medium.com/@calebleak/fast-virtual-functions-hacking-the-vtable-for-fun-and-profit-25c36409c5e0

	//Simple check function return bool value with no parameters
	typedef bool (BoolFn)();
	
	//Simple function returning nothing with no parameters
	typedef void (VoidFn)();
	
	//Simple function returning nothing with integer parameter
	typedef void (Void_IntFn)(const int&);
	
	//Simple function returning nothing with string parameter
	typedef void (Void_StringFn)(const std::string&);
	
	//Function Returning RepType
	typedef RepType (RepTypeFn)();
	
	//Function Returning RepType
	typedef std::string (String_RepTypeFn)(const RepType&);
	
/*
#if defined(AltNum_UseBuiltinVirtualTable)
	protected:
		template <typename T>
		void** GetVTable(T* obj) {
			return *((void***)obj);
		}
#endif
*/
}