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
	struct MirroredIntV2 {
	#pragma options align=bit_packed
	//If value is one then is negative
	unsigned int IsPositive:1;
	//Stores non-signed part of value
	unsigned int Value:31;
	#pragma options align=reset
		MirroredIntV2(const unsigned int& value=0, const unsigned int& isPositive=1)
		{
			Value = value;
			isPositive = isPositive;
		}
		MirroredIntV2(const unsigned int& value=0)
		{
			if(value<0)
			{
				isPositive = 0;
				Value = -value;
			}
			else
				isPositive = 1;
			
		}
		bool IsNegative()
		{
			return IsPositive!=1;
		}
		std::strong_ordering operator<=>(const PartialInt& that) const
		{
			//Comparing if number is negative vs positive
			if (auto SignCmp = IsPositive <=> that.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		bool operator==(const unsigned int& that) const
		{
			if (Value!=that)
				return false;
			return true;
		}
	};
}