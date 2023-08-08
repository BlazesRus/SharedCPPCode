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

BlazesMirroredInt_PreventNZeroUnderflowCheck
*/

//Auto toggles to default option if not set
#if !defined(BlazesMirroredInt_UsePseudoBitSet) && !defined(BlazesMirroredInt_UseBitSet) && !defined(BlazesMirroredInt_UseLegacyValueBehavior)
	#define BlazesMirroredInt_UseLegacyValueBehavior
#endif

namespace BlazesRusCode
{
    class MirroredInt;
	//Int but instead with Negative Zero behind zero (for use in fixed point decimal-like format)
    class DLL_API MirroredInt
    {
	public:
#if defined(BlazesMirroredInt_UseLegacyValueBehavior)
        static signed int const NegativeRep = -2147483648;
#elif defined(BlazesMirroredInt_UsePseudoBitSet)
        static unsigned int const NegativeRep = 2147483648;
#elif defined(BlazesMirroredInt_UseBitSet)
        //static const NegativeRep = ;
#endif
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
		unsigned int Value;
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
		signed int Value;
	#elif defined(BlazesMirroredInt_UseBitSet)
        // Value;
	#endif
	
        /// <summary>
        /// Initializes a new instance of the <see cref="MirroredInt"/> class.
        /// </summary>
        /// <param name="Tvalue">The value.</param>
        MirroredInt(int Tvalue = 0)
        {
            this->SetVal(Tvalue);
        }

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

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(int TValue)
        {
            Value = TValue;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MirroredInt TValue)
        {
            Value = TValue.Value;
        }

        void SetAsZero()
        {
			Value = 0;
        }

        void SetAsNegativeZero()
        {
			Value = NegativeRep;
        }

#pragma region ValueDefines
        //Value at exactly zero(but not at negative side)
        static MirroredInt ZeroValue()
        {
            MirroredInt NewLValue = MirroredInt();
            return NewLValue;
        }

        static MirroredInt NegativeZeroValue()
        {
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
            MirroredInt NewLValue = MirroredInt(2147483648);
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            MirroredInt NewLValue = MirroredInt(-2147483648);
	#else
            //Set bit flags to 10000000 00000000 00000000 00000000
	#endif
            return NewLValue;
        }

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt Zero;

        /// <summary>
        /// Returns the value at negative zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeZero;

        static MirroredInt OneValue()
        {
            MirroredInt NewLValue = MirroredInt(1);
            return NewLValue;
        }

        static MirroredInt One;
#pragma endregion ValueDefines

#pragma region Comparison Operators
        /// <summary>
        /// Lesser than Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MirroredInt LValue, MirroredInt RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
                return RValue.Value < 0 && RValue.Value != NegativeRep;
            else if (RValue.Value == NegativeRep)
            {
                if (LValue.Value < 0)
                    return true;
                else
                    return false;
            }
            else
                return LValue.Value < RValue.Value;
#else
#endif
        }

        /// <summary>
        /// Lesser than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MirroredInt LValue, MirroredInt RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
                return RValue.Value < 0;
            else if (RValue.Value == NegativeRep)
            {
                if (LValue.Value >= 0)
                    return false;
                else
                    return true;
            }
            else
                return LValue.Value <= RValue.Value;
#else
#endif
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>(MirroredInt LValue, MirroredInt RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
            {
                if (RValue.Value < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.Value == NegativeRep)
            {
                if (LValue.Value >= 0)
                    return true;
                else
                    return false;
            }
            else
                return LValue.Value > RValue.Value;
#else
#endif
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MirroredInt LValue, MirroredInt RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == RValue.Value)
                return true;
            if (LValue.Value == NegativeRep)
            {
                if (RValue.Value < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.Value == NegativeRep)
            {
                if (LValue.Value >= 0)
                    return true;
                else
                    return false;
            }
            else
                return LValue.Value > RValue.Value;
#else
#endif
        }

        /// <summary>
        /// Not Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue.Value != RValue.Value;
        }

        /// <summary>
        /// Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue.Value == RValue.Value;
        }

        /// <summary>
        /// Lesser than Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MirroredInt LValue, int RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
                return RValue < 0 && RValue != NegativeRep;
            else
                return LValue.Value < RValue;
#else
#endif
        }

        /// <summary>
        /// Lesser than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MirroredInt LValue, int RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
                return RValue < 0;
            else
                return LValue.Value <= RValue;
#else
#endif
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>(MirroredInt LValue, int RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == NegativeRep)
            {
                if (RValue < 0)
                    return false;
                else
                    return true;
            }
            else
                return LValue.Value > RValue;
#else
#endif
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MirroredInt LValue, int RValue)
        {
#if defined(BlazesMirroredInt_UsePseudoBitSet)
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            if (LValue.Value == RValue)
                return true;
            if (LValue.Value == NegativeRep)
            {
                if (RValue < 0)
                    return false;
                else
                    return true;
            }
            else
                return LValue.Value > RValue;
#else
#endif
        }

        /// <summary>
        /// Not Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MirroredInt LValue, int RValue)
        {
            return LValue.Value != RValue;
        }

        /// <summary>
        /// Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MirroredInt LValue, int RValue)
        {
            return LValue.Value == RValue;
        }

        friend bool operator!=(int LValue, MirroredInt RValue)
        {
            return RValue != LValue;
        }

        friend bool operator==(int LValue, MirroredInt RValue)
        {
            return RValue == LValue;
        }

        friend bool operator<(int LValue, MirroredInt RValue)
        {
            return RValue > LValue;
        }

        friend bool operator<=(int LValue, MirroredInt RValue)
        {
            return RValue >= LValue;
        }

        friend bool operator>(int LValue, MirroredInt RValue)
        {
            return RValue < LValue;
        }

        friend bool operator>=(int LValue, MirroredInt RValue)
        {
            return RValue <= LValue;
        }
#pragma endregion Comparison Operators

#pragma region IntegerOperations//Including Mirrored Int Operations

        /// <summary>
        /// Addition Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator+=(MirroredInt& LValue, IntType& RValue)
        {
            if(RValue==0)
                return LValue;
            if(LValue.Value==0)
                LValue.Value = RValue;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
            else
			    throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            else if(LValue.Value==NegativeRep)
            {
                //-0.XXXX + 2 = 1.XXXX
                //-0.XXXX + 1 = 0.XXXX (Flips to other side)
                //-0.XXXX + -5 = -5.XXXX
                if(RValue<0)
                {
                    LValue.Value = RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
                else//(RValue>=0)
                    LValue.Value = RValue - 1;
            }
			else if(LValue.Value<0)
            {
                //-1.XXXX + -5 = -6.XXXX
                //-6.XXXX + 5 = -1.XXXX
                //-6.XXXX + 6 = -0.XXXX
                //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                //-5.XXXX + 7 = 1.XXXX
                int InvertedLValue = -LValue.Value;
                if(LValue.Value==InvertedLValue)
                    LValue.Value = NegativeRep;
                else if(RValue>InvertedLValue)
                    LValue.Value += RValue-1;
                else
                {
                    LValue.Value += RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
            else
            {
                //5.XXXX + 5 = 10.XXXX
                //5.XXXX + -5 = 0.XXXX
                //5.XXXX + -6 = -0.XXXX (Flips to other side)
                //5.XXXX + -7 = -1.XXXX 
                int InversionPoint = -LValue.Value - 1;
                if(RValue==InversionPoint)
                    LValue.Value = NegativeRep;
                else if(RValue<InversionPoint)
                {
                    LValue.Value += RValue + 1;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
                else
                    LValue.Value += RValue;
            }
	#else
            else
			    throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator+=(MirroredInt& LValue, MirroredInt& RValue)
        {
            if(RValue.IsZero())
                return LValue;
            if(LValue.Value==0)
                LValue.Value = RValue.Value;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
            else
			    throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            else if(LValue.Value==NegativeRep)
            {
                //-0.XXXX + 2 = 1.XXXX
                //-0.XXXX + 1 = 0.XXXX (Flips to other side)
                //-0.XXXX + -5 = -5.XXXX
                if(RValue<0)
                {
                    LValue.Value = RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
                else//(RValue>=0)
                    LValue.Value = RValue.Value - 1;
            }
			else if(LValue.Value<0)
            {
                //-1.XXXX + -5 = -6.XXXX
                //-6.XXXX + 5 = -1.XXXX
                //-6.XXXX + 6 = -0.XXXX
                //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                //-5.XXXX + 7 = 1.XXXX
                int InvertedLValue = -LValue.Value;
                if(LValue.Value==InvertedLValue)
                    LValue.Value = NegativeRep;
                else if(RValue>InvertedLValue)
                    LValue.Value += RValue.Value-1;
                else
                {
                    LValue.Value += RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
            else
            {
                //5.XXXX + 5 = 10.XXXX
                //5.XXXX + -5 = 0.XXXX
                //5.XXXX + -6 = -0.XXXX (Flips to other side)
                //5.XXXX + -7 = -1.XXXX 
                int InversionPoint = -LValue.Value - 1;
                if(RValue==InversionPoint)
                    LValue.Value = NegativeRep;
                else if(RValue<InversionPoint)
                {
                    LValue = RValue + 1;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
                else
                    LValue.Value += RValue.Value;
            }
	#else
            else
			    throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        void NRepSkippingAddOp(MirroredInt& LValue, IntType& RValue)
        {
            if(RValue==0)
                return;
            if(LValue.Value==0)
                LValue.Value = RValue.Value;
            else
            {
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value += RValue;
        #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                if(RValue<0&&LValue.Value==NegativeRep)
                    throw "MirroredInt value has underflowed";
        #endif
    #else
			    throw "Need to write code for operation";//Placeholder
    #endif
            }
			return;
        }

        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        void NRepSkippingAddOp(MirroredInt& LValue, MirroredInt& RValue)
        {
            if(RValue.IsZero())
                return;
            if(LValue.Value==0)
                LValue.Value = RValue.Value;
            else
            {
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value += RValue.Value;
        #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                if(RValue<0&&LValue.Value==NegativeRep)
                    throw "MirroredInt value has underflowed";
        #endif
    #else
			    throw "Need to write code for operation";//Placeholder
    #endif
            }
			return;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator+(MirroredInt LValue, IntType RValue)
        {
            return LValue+=RValue;
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator+(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue+=RValue;
        }

        template<typename IntType>
        friend MirroredInt operator+(IntType LValue, MirroredInt RValue)
        {
            MirroredInt Res = MirroredInt(LValue) + RValue;
            return RValue;
        }

        template<typename IntType>
        friend MirroredInt operator+=(IntType& LValue, MirroredInt RValue)
        {
            LValue = LValue + RValue;
        }

        /// <summary>
        /// Subtraction Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator-=(MirroredInt& LValue, IntType RValue)
        {
            if(RValue==0)
                return LValue;
            if(LValue.Value==0)
                LValue.Value = -RValue;
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
            else
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            else if(LValue.Value==NegativeRep)
            {
                //-0.XXXX - -2 = 1.XXXX
                //-0.XXXX - -1 = 0.XXXX (Flips to other side)
                //-0.XXXX - 1 = -1.XXXX 
                //-0.XXXX - 6 = -6.XXXX
                if(RValue<0)
                    LValue.Value = -RValue - 1;
                else
                {
                    LValue.Value = -RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
			else if(LValue.Value<0)
            {
                //-5.XXXX - -7 = 1.XXXX 
                //-5.XXXX - -6 = 0.XXXX            
                //-5.XXXX - -5 = -0.XXXX (Flips to other side)
                //-5.XXXX - -4 = -1.XXXX
                if(RValue==LValue.Value)
                    LValue.Value = NegativeRep;
                else if(RValue<LValue.Value)
                    LValue.Value -= RValue.Value - 1;
                else//(RValue>=LValue.Value)
                {
                    LValue.Value -= RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
            else
            {
                //5.XXXX - -5 = 10.XXXX
                //5.XXXX - 5 = 0.XXXX
                //5.XXXX - 6 = -0.XXXX (Flips to other side)
                //5.XXXX - 7 = -1.XXXX
                int InversionPoint = LValue.Value + 1;
                if(RValue==InversionPoint)
                    LValue.Value = NegativeRep;
                else if(RValue>InversionPoint)
                {
                    LValue.Value -= RValue - 1;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                } 
                else
                    LValue.Value += RValue;
            }
    #else
            else
			    throw "Need to write code for operation";//Placeholder
    #endif
			return LValue;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator-=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(RValue.IsZero())
                return LValue;
            if(LValue.Value==0)
                LValue.Value = -RValue.Value;
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
            else
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
            else if(LValue.Value==NegativeRep)
            {
                //-0.XXXX - -2 = 1.XXXX
                //-0.XXXX - -1 = 0.XXXX (Flips to other side)
                //-0.XXXX - 1 = -1.XXXX 
                //-0.XXXX - 6 = -6.XXXX
                if(RValue<0)
                    LValue.Value = -RValue.Value - 1;
                else
                {
                    LValue.Value = -RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
			else if(LValue.Value<0)
            {
                //-5.XXXX - -7 = 1.XXXX 
                //-5.XXXX - -6 = 0.XXXX            
                //-5.XXXX - -5 = -0.XXXX (Flips to other side)
                //-5.XXXX - -4 = -1.XXXX
                if(RValue==LValue.Value)
                    LValue.Value = NegativeRep;
                else if(RValue<LValue.Value)
                    LValue.Value -= RValue.Value - 1;
                else//(RValue>=LValue.Value)
                {
                    LValue.Value -= RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                }
            }
            else
            {
                //5.XXXX - -5 = 10.XXXX
                //5.XXXX - 5 = 0.XXXX
                //5.XXXX - 6 = -0.XXXX (Flips to other side)
                //5.XXXX - 7 = -1.XXXX
                int InversionPoint = LValue.Value + 1;
                if(RValue==InversionPoint)
                    LValue.Value = NegativeRep;
                else if(RValue>InversionPoint)
                {
                    LValue.Value -= RValue.Value - 1;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if(LValue.Value==NegativeRep)
                        throw "MirroredInt value has underflowed";
		#endif
                } 
                else
                    LValue.Value += RValue.Value;
            }
#else
            else
			    throw "Need to write code for operation";//Placeholder
#endif
			return LValue;
        }

        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        void NRepSkippingSubOp(MirroredInt& LValue, int& RValue)
        {
            if(RValue.IsZero())
                return;
            if(LValue.Value==0)
                LValue.Value = -RValue.Value;
            else
            {
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value -= RValue.Value;
        #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                if(RValue<0&&LValue.Value==NegativeRep)
                    throw "MirroredInt value has underflowed";
        #endif
    #else
			throw "Need to write code for operation";//Placeholder
    #endif
            }
			return;
        }

        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        void NRepSkippingSubOp(MirroredInt& LValue, MirroredInt& RValue)
        {
            if(RValue.IsZero())
                return;
            if(LValue.Value==0)
                LValue.Value = -RValue.Value;
            else
            {
    #if defined(BlazesMirroredInt_UsePseudoBitSet)
			    throw "Need to write code for operation";//Placeholder
    #elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value -= RValue.Value;
        #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                if(RValue<0&&LValue.Value==NegativeRep)
                    throw "MirroredInt value has underflowed";
        #endif
    #else
			    throw "Need to write code for operation";//Placeholder
    #endif
            }
			return;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator-(MirroredInt LValue, IntType RValue)
        {
            return LValue-=RValue;
        }

        template<typename IntType>
        friend MirroredInt operator-(IntType LValue, MirroredInt RValue)
        {
            MirroredInt Res = MirroredInt(LValue) - RValue;
            return Res;
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator-(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue-=RValue;
        }

        template<typename IntType>
        friend MirroredInt operator-=(IntType& LValue, MirroredInt RValue)
        {
            LValue = RValue - LValue;
        }

        /// <summary>
        /// Multiplication Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator*=(MirroredInt& LValue, IntType RValue)
        {
            if(LValue.IsZero()||RValue==0)
                return LValue;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			LValue.Value *= RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
            if(LValue.Value==NegativeRep)
				throw "MirroredInt value has underflowed";
		#endif
	#else
			throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator*=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(LValue.IsZero()||RValue.IsZero())
                return LValue;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			LValue.Value *= RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
            if(LValue.Value==NegativeRep)
				throw "MirroredInt value has underflowed";
		#endif
	#else
			throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator*(MirroredInt LValue, IntType RValue)
        {
            return LValue*=RValue;
        }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator*(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue*=RValue;
        }

        template<typename IntType>
        friend MirroredInt operator*=(IntType& LValue, MirroredInt RValue)
        {
            if (RValue.IsZero())
                LValue = 0;
            else
                LValue *= RValue.Value;
        }

        /// <summary>
        /// Division Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator/=(MirroredInt& LValue, IntType RValue)
        {
			if(RValue==0)
				throw "Can't divide by zero without infinity result";
			if(LValue.IsZero())
				return LValue;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			LValue.Value /= RValue;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
            if(LValue.Value==NegativeRep)
				throw "MirroredInt value has underflowed";
		#endif
	#else
			throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        template<typename IntType>
        friend MirroredInt operator/=(IntType& LValue, MirroredInt RValue)
        {
            if (RValue.IsZero())
                throw "Can't divide by zero without infinity result";
            LValue /= RValue.Value;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator/=(MirroredInt& LValue, MirroredInt RValue)
        {
			if(RValue.IsZero())
				throw "Can't divide by zero without infinity result";
			if(LValue.IsZero())
				return LValue;
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
			throw "Need to write code for operation";//Placeholder
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			LValue.Value /= RValue.Value;
		#if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
            if(LValue.Value==NegativeRep)
				throw "MirroredInt value has underflowed";
		#endif
	#else
			throw "Need to write code for operation";//Placeholder
	#endif
			return LValue;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator/(MirroredInt LValue, IntType RValue)
        {
            return LValue/=RValue;
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator/(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue/=RValue;
        }

        /// <summary>
        /// %= Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator%=(MirroredInt& LValue, IntType RValue)
        {
            if(LValue.IsZero())
                LValue.Value = 0;
            else
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            {
                if(IsNegative)
                    throw "Need to write code for operation with negative number";//Placeholder
                else
                    LValue.Value%= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value%=RValue;
#else
                throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// %= Operation with Integer Value
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator%=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(LValue.IsZero())
                LValue.Value = 0; 
            else
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            {
                if(IsNegative)
                    throw "Need to write code for operation with negative number";//Placeholder
                else
                    LValue.Value%= RValue.Value;
            }
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value%=RValue.Value;
#else
                throw "Need to write code for operation with negative number";//Placeholder
#endif 
            return LValue;
        }

        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator%(MirroredInt LValue, IntType RValue)
        {
            return LValue%=RValue;
        }

        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator%(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue%=RValue;
        }

#pragma endregion IntegerOperations

#pragma region BitwiseOperations

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator^=(MirroredInt& LValue, IntType RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                LValue.Value^= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value^=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator^=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                LValue.Value^= RValue.Value;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value^=RValue.Value;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator^(MirroredInt LValue, IntType RValue)
        {
            return LValue^=RValue;
        }

        /// <summary>
        /// Bitwise XOR Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator^(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue^=RValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator|=(MirroredInt& LValue, IntType RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                LValue.Value|= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value|=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator|=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                LValue.Value|= RValue.Value;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value|=RValue.Value;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator|(MirroredInt LValue, IntType RValue)
        {
            return LValue|=RValue;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator|(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue|=RValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator&=(MirroredInt& LValue, IntType RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;

#if defined(BlazesMirroredInt_UsePseudoBitSet)
            if(IsNegative)
                throw "Need to write code for operation with negative number";//Placeholder
            else
                LValue.Value&= RValue;
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value&=RValue;
#else
            throw "Need to write code for operation with negative number";//Placeholder
#endif
                return LValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator&=(MirroredInt& LValue, MirroredInt RValue)
        {
            if(LValue==NegativeZero)
                LValue.Value = 0;
            else
#if defined(BlazesMirroredInt_UsePseudoBitSet)
            {
                if(IsNegative)
                    throw "Need to write code for operation with negative number";//Placeholder
                else
                    LValue.Value&= RValue;
            }
#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
                LValue.Value&=RValue.Value;
#else
                throw "Need to write code for operation with negative number";//Placeholder
#endif
            return LValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        friend MirroredInt operator&(MirroredInt LValue, IntType RValue)
        {
            return LValue&=RValue;
        }

        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt operator&(MirroredInt LValue, MirroredInt RValue)
        {
            return LValue&=RValue;
        }

#pragma endregion BitwiseOperations

#pragma region OtherOperators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <returns>MirroredInt</returns>
        friend MirroredInt& operator-(MirroredInt& LValue)
        {
	#if defined(BlazesMirroredInt_UsePseudoBitSet)
			if(LValue.Value==0)
				LValue.Value = NegativeRep;
			else if(LValue.Value==NegativeRep)
				LValue.Value = 0;
			else if(Value>NegativeRep)
				LValue.Value -= NegativeRep;
			else
				LValue.Value += NegativeRep;
	#elif defined(BlazesMirroredInt_UseLegacyValueBehavior)
			if(LValue.Value==0)
				LValue.Value = NegativeRep;
			else if(LValue.Value==NegativeRep)
				LValue.Value = 0;
			else
				LValue.Value *= -1;
	#else
	#endif
			return LValue;
        }
public:
        void AddOp(MirroredInt& LValue, MirroredInt& RValue)
        {
            LValue += RValue;
        }

        void SubOp(MirroredInt& LValue, MirroredInt& RValue)
        {
            LValue -= RValue;
        }

        /// <summary>
        /// ++Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator ++()
        {
            AddOp(*this, One);
            return *this;
        }

        /// <summary>
        /// ++Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator --()
        {
            SubOp(*this, One);
            return *this;
        }

        /// <summary>
        /// MirroredInt++ Operator
        /// </summary>
        /// <returns>MirroredInt</returns>
        MirroredInt& operator ++(int)
        {
            MirroredInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt-- Operator
        /// </summary>
        /// <returns>MirroredInt</returns>
        MirroredInt& operator --(int)
        {
            MirroredInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt* Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator *()
        {
            return *this;
        }
#pragma endregion OtherOperators

#pragma region StringOperations
#pragma endregion StringOperations

#pragma region ConversionToType
        /// <summary>
        /// MirroredInt to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int()
		{
			if(IsZero)
				return 0;
			return Value;
		}
		
		/// <summary>
        /// MirroredInt to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string()
		{
			if(Value==NegativeRep)
				return "-0";
			return VariableConversionFunctions::IntToStringConversion(Value);
		}
#pragma endregion ConversionToType

#pragma region ConversionFromType
#pragma endregion ConversionFromType
    };

#pragma region ValueDefine Source
    MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();
    MirroredInt MirroredInt::NegativeZero = MirroredInt::NegativeZeroValue();
    MirroredInt MirroredInt::One = MirroredInt::OneValue();
#pragma endregion ValueDefine Source
}
