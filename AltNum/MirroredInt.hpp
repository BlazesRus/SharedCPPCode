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
BlazesMirroredInt_UseLegacyValueBehavior:
-  stores value in signed int

BlazesMirroredInt_UseBitSet:
-  stores value in bitset with bitfield of 4

BlazesMirroredInt_UsePseudoBitSet:(32nd bit used for positive/negative status Bits 1-31 are reversed order from signed int)
-  10000000 00000000 00000000 00000000 = Negative Zero (same bit values for negative zero as used in BlazesMirroredInt_UseLegacyValueBehavior)
-  stores value in unsigned int

*/

//Auto toggles to default option if not set
#if !defined(BlazesMirroredInt_UsePseudoBitSet) && !defined(BlazesMirroredInt_UseBitSet) && !defined(BlazesMirroredInt_UseLegacyValueBehavior)
	#define BlazesMirroredInt_UseLegacyValueBehavior
#endif

namespace BlazesRusCode
{
    class MirroredInt;
	//Int but instead with Negative Zero behind zero (for use in fixed point decimal-like format)
    class DLL_API AltDec
    {
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
	private:
    #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
        static signed int NegativeRep = -2147483648;
    #elif defined(BlazesMirroredInt_UsePseudoBitSet)
        static unsigned int NegativeRep = 2147483648;
    #endif
	public:
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
		unsigned int Value;
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
		signed int Value;
	#elif defined(BlazesMirroredInt_UseBitSet)
	#endif
        //Returns true if detected as either zero or negative zero value
		bool IsZero()
		{
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(Value==0||Value==2147483648)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if(Value==0||Value==-2147483648)
#else
            //If bits# 1 - 31(index 0 - 30) are zero, then value is zero or Negative Zero
#endif
                return true;
            return false;
		}
		
		bool IsNegative()
		{
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(Value>2147483647)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if(Value<0)
#else
            //check 32nd bit to see if negative
#endif
                return true;
            return false;
		}

        //Return value as real number(negative zero counts as zero)
        int GetValue()
        {
            if(IsZero())
                return 0;
#if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            return Value;
#elif defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative())
            {
                return -(signed int)(Value - 2147483648);
            }
            else
                return (signed int)Value;
#else
#endif      
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
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            MirroredInt NewSelf = MirroredInt(-2147483648);
#else
            //Set bit flags to 10000000 00000000 00000000 00000000
#endif
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeZero;

#pragma endregion ValueDefines

#pragma region IntegerOperations//Including Mirrored Int Operations

        /// <summary>
        /// Addition Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator+=(MirroredInt& self, IntType& RValue)
        {
            if(RValue==0)
                return self;
            if(self.Value==0)
                self.Value = RValue;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if(self.Value==NegativeRep)
            {
                if(RValue<0)//-0.XXXX + -5 = -5.XXXX
                {
                    self.Value = RValue;
                    if(self.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
                }
                else//-0.XXXX + 5 = -4.XXXX
                    self.Value = RValue - 1;
            }
			else
            {
                if(self.Value<0)
                {
                    int InvertedSelf = -self.Value;
                    if(self.Value==InvertedSelf)
                        self.Value = NegativeRep;
                    //-1.XXXX + -5 = -6.XXXX
                    //-6.XXXX + 5 = -1.XXXX
                    //-6.XXXX + 6 = -0.XXXX
                    //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                    //-5.XXXX + 7 = 1.XXXX
                    else if(RValue>InvertedSelf)
                        self.Value += RValue-1;
                    else
                    {
                        self.Value += RValue;
                        if(self.Value==NegativeRep)
                            throw "MirroredInt value has underflowed";
                    }
                }
                else
                {
                    int InversionPoint = -self.Value - 1;
                    if(RValue==InversionPoint)
                        self.Value = NegativeRep;
                    else if(RValue<InversionPoint)
                    {
                        self.Value -= RValue + 1;
                        if(self.Value==NegativeRep)
                            throw "MirroredInt value has underflowed";
                    }
                    //5.XXXX + 5 = 10.XXXX
                    //5.XXXX + -5 = 0.XXXX
                    //5.XXXX + -6 = -0.XXXX (Flips to other side)
                    //5.XXXX + -7 = -1.XXXX 
                    else
                        self.Value += Value;
                }
            }
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator+=(MirroredInt& self, MirroredInt& RValue)
        {
            if(RValue.IsZero())
                return self;
            if(self.Value==0)
                self.Value = RValue.Value;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if(self.Value==NegativeRep)
            {
                if(RValue<0)//-0.XXXX + -5 = -5.XXXX
                {
                    self.Value = RValue.Value;
                    if(self.Value==NegativeRep)
                            throw "MirroredInt value has underflowed";
                }
                else//-0.XXXX + 5 = -4.XXXX
                    self.Value = RValue.Value - 1;
            }
			else if(self.Value<0)
            {
                int InvertedSelf = -self.Value;
                if(self.Value==InvertedSelf)
                    self.Value = NegativeRep;
                else
                {
                    //-1.XXXX + -5 = -6.XXXX
                    //-6.XXXX + 5 = -1.XXXX
                    //-6.XXXX + 6 = -0.XXXX
                    //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                    //-5.XXXX + 7 = 1.XXXX
                    if(RValue>InvertedSelf)
                        self.Value += RValue.Value-1;
                    else
                    {
                        self.Value += RValue.Value;
                        if(self.Value==NegativeRep)
                                throw "MirroredInt value has underflowed";
                    }
                }
            }
            else
            {
                int InversionPoint = -self.Value - 1;
                if(RValue==InversionPoint)
                    self.Value = NegativeRep;
                else if(RValue<InversionPoint)
                {
                    self.Value += RValue + 1;
                    if(self.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
                }
                //5.XXXX + 5 = 10.XXXX
                //5.XXXX + -5 = 0.XXXX
                //5.XXXX + -6 = -0.XXXX (Flips to other side)
                //5.XXXX + -7 = -1.XXXX 
                else
                    self.Value += RValue.Value;
            }
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        void MirroredInt NRepSkippingAddOp(MirroredInt& self, MirroredInt& RValue)
        {
            if(RValue.IsZero())
                return self;
            if(self.Value==0)
                self.Value = RValue.Value;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            self.Value += RValue.Value;
            if(RValue<0&&self.Value==NegativeRep)
                throw "MirroredInt value has underflowed";
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator+(MirroredInt self, IntType RValue)
        {
            return self+=RValue;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator+(MirroredInt self, MirroredInt RValue)
        {
            return self+=RValue;
        }

        /// <summary>
        /// Subtraction Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator-=(MirroredInt& self, IntType RValue)
        {
            if(RValue==0)
                return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			throw "Need to write code for operation";//Placeholder
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator-=(MirroredInt& self, MirroredInt RValue)
        {
            if(RValue.IsZero())
                return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			throw "Need to write code for operation";//Placeholder
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator-(MirroredInt self, IntType RValue)
        {
            return self-=RValue;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator-(MirroredInt self, MirroredInt RValue)
        {
            return self-=RValue;
        }

        /// <summary>
        /// Multiplication Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator*=(MirroredInt& self, IntType RValue)
        {
            if(self.IsZero()||RValue==0)
                return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			self.Value *= RValue;
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator*=(MirroredInt& self, MirroredInt RValue)
        {
            if(self.IsZero()||RValue.IsZero())
                return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			self.Value *= RValue.Value;
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator*(MirroredInt self, IntType RValue)
        {
            return self*=RValue;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator*(MirroredInt self, MirroredInt RValue)
        {
            return self*=RValue;
        }

        /// <summary>
        /// Division Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator/=(MirroredInt& self, IntType RValue)
        {
			if(RValue==0)
				throw "Can't divide by zero without infinity result";
			if(self.IsZero())
				return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			self.Value /= RValue;
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator/=(MirroredInt& self, MirroredInt RValue)
        {
			if(RValue==0)
				throw "Can't divide by zero without infinity result";
			if(self.IsZero())
				return self;
#if defined(BlazesMirroredInt_UsePseudoBitSet)
			if(RValue.IsNegative())
            {
            }
            else
            {
            }
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			self.Value /= RValue;
#else
			throw "Need to write code for operation";//Placeholder
#endif
			return self;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator/(MirroredInt self, IntType RValue)
        {
            return self/=RValue;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator/(MirroredInt self, MirroredInt RValue)
        {
            return self/=RValue;
        }

        /// <summary>
        /// %= Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator%=(MirroredInt& self, IntType RValue)
        {
            if(self.IsZero())
                self.Value = 0;
            else
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            {
                if(IsNegative)
                    throw "Need to write code for operation with negative number";//Placeholder
                else
                    self.Value%= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value%=RValue;
#else
                throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// %= Operation with Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator%=(MirroredInt& self, MirroredInt RValue)
        {
            if(self.IsZero())
                self.Value = 0; 
            else
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            {
                if(IsNegative)
                    throw "Need to write code for operation with negative number";//Placeholder
                else
                    self.Value%= RValue.Value;
            }
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value%=RValue.Value;
#else
                throw "Need to write code for operation with negative number";//Placeholder
#endif 
            return self;
        }

        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator%(MirroredInt self, IntType RValue)
        {
            return self%=RValue;
        }

        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator%(MirroredInt self, MirroredInt RValue)
        {
            return self%=RValue;
        }

#pragma endregion IntegerOperations

#pragma region BitwiseOperations

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator^=(MirroredInt& self, IntType RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value^= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator^=(MirroredInt& self, MirroredInt RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value^= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator^(MirroredInt self, IntType RValue)
        {
            return self^=RValue;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator^(MirroredInt self, MirroredInt RValue)
        {
            return self^=RValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator|=(MirroredInt& self, IntType RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value|= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator|=(MirroredInt& self, MirroredInt RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value|= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator|(MirroredInt self, IntType RValue)
        {
            return self|=RValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator|(MirroredInt self, MirroredInt RValue)
        {
            return self|=RValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator&=(MirroredInt& self, IntType RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value&= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator&=(MirroredInt& self, MirroredInt RValue)
        {
            if(self==NegativeZero)
                self.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                self.Value&= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                self.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return self;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredIntVariant operator&(MirroredInt self, IntType RValue)
        {
            return self&=RValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredIntVariant operator&(MirroredInt self, MirroredInt RValue)
        {
            return self&=RValue;
        }

#pragma endregion BitwiseOperations

#pragma region StringOperations
#pragma endregion StringOperations

#pragma region ConversionToType
#pragma endregion ConversionToType

#pragma region ConversionFromType
#pragma endregion ConversionFromType
	}

#pragma region ValueDefine Source
    MirroredIntVariant MirroredInt::Zero = ZeroValue();
    MirroredIntVariant MirroredInt::NegativeZero = OneValue();
#pragma endregion ValueDefine Source
}
	