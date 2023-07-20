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

//Preprocessor options
/*
BlazesMirroredInt_UseLegacyIntValueBehavior:
-  stores value in signed int

BlazesMirroredInt_UseBitSet:
-  stores value in bitset with bitfield of 4

BlazesMirroredInt_UsePseudoBitSet:(32nd bit used for positive/negative status Bits 1-31 are reversed order from signed int)
-  10000000 00000000 00000000 00000000 = Negative Zero (same bit values for negative zero as used in BlazesMirroredInt_UseLegacyIntValueBehavior)
-  stores value in unsigned int

*/

//Auto toggles to default option if not set
#if !defined(BlazesMirroredInt_UsePseudoBitSet) && !defined(BlazesMirroredInt_UseBitSet) && !defined(BlazesMirroredInt_UseLegacyIntValueBehavior)
	#define BlazesMirroredInt_UsePseudoBitSet
#endif

namespace BlazesRusCode
{
    class MirroredInt;
	//Int but instead with Negative Zero behind zero (for use in fixed point decimal-like format)
    class DLL_API AltDec
    {
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
	private:
		unsigned int Value;
	#elif defined(BlazesMirroredInt_UseLegacyIntValueBehavior)
		signed int Value;
	#elif defined(BlazesMirroredInt_UseBitSet)
	#endif
	public:
        //Returns true if detected as either zero or negative zero value
		bool IsZero()
		{
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(Value==0||Value==2147483648)
#elif defined(BlazesMirroredInt_UseLegacyIntValueBehavior)
            if(Value==0||Value==-2147483648)
#else
            //If bits# 1 - 31 are zero, then value is zero or Negative Zero
#endif
                return true;
            return false;
		}
		
		bool IsNegative()
		{
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(Value>2147483647)
#elif defined(BlazesMirroredInt_UseLegacyIntValueBehavior)
            if(Value<0)
#else
            //check 32nd bit to see if negative
#endif
                return true;
            return false;
		}
#pragma region ValueDefines
        //Value at exactly zero(but not at negative side)
        static MirroredInt ZeroValue()
        {
            MirroredInt NewSelf = MirroredInt();
            return NewSelf;
        }

        static MirroredInt NegativeZeroValue()
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            MirroredInt NewSelf = MirroredInt(2147483648);
#elif defined(BlazesMirroredInt_UseLegacyIntValueBehavior)
            MirroredInt NewSelf = MirroredInt(-2147483648);
#else
            //Set bit flags to 10000000 00000000 00000000 00000000
#endif
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MirroredInt Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MirroredInt NegativeZero;

#pragma endregion ValueDefines
	}

#pragma region ValueDefine Source
    MediumDecVariant MirroredInt::Zero = ZeroValue();
    MediumDecVariant MirroredInt::NegativeZero = OneValue();
#pragma endregion ValueDefine Source
}
	