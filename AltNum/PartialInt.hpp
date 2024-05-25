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
		
		std::strong_ordering operator<=>(const PartialInt& that) const
		{
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		
		bool operator==(const unsigned int& that) const
		{
			if (Value!=that)
				return false;
			return true;
		}
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return Value; }
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator UInt64() { return Value; }
	};
}