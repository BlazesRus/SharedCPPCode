// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

namespace BlazesRusCode
{
//Forward calling virtual table based on https://medium.com/@calebleak/fast-virtual-functions-hacking-the-vtable-for-fun-and-profit-25c36409c5e0

	//Simple check function return bool value with no parameters
	typedef void (CheckFn)();
	
	//Simple function returning nothing with no parameters
	typedef void (VoidFn)();
	
	//Simple function returning nothing with integer parameter
	typedef void (VoidIntFn)(const int&);

	struct AltNum_VTable {
 
	};
}