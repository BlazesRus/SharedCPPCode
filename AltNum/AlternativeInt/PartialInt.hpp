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

#include <compare>

namespace BlazesRusCode
{
	class PartialInt {
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
		
        //Is at either zero or negative zero
		bool IsAtZeroInt() const
        {
			return Value==0;
        }

        //Is at neither zero or negative zero
		bool IsNotAtZeroInt() const
        {
			return Value!=0;
        }
		
        //Is at either zero or negative one
		bool IsAtOneInt() const
        {
			return Value==0;
        }

        //Is at neither zero or negative one
		bool IsNotAtOneInt() const
        {
			return Value!=0;
        }
		
		bool IsEven() const
		{
			return (Value^1)==0;
		}
		
		bool IsOdd() const
		{
			return (Value^1)==1;
		}

		std::strong_ordering operator<=>(const PartialInt& that) const
		{
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		
		bool operator==(const PartialInt& that) const
		{
			if (Value!=that.Value)
				return false;
            if(Flags!=that.Flags)
                return false;
			return true;
		}

		std::strong_ordering operator<=>(const unsigned int& that) const
		{
			if (auto ValueCmp = Value <=> that; ValueCmp != 0)
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
        explicit operator unsigned long long() { return Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return Value; }
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed long long() { return Value; }

protected:
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt ZeroValue()
        {
            return MirroredInt(0,1);
        }
public:

		static MirroredInt Zero;

	};

	MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();

}