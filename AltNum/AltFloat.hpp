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

#include "..\IntegerConcept.hpp"

#if defined(AltFloat_DontUseTinyUDec)
	#include "TinyUDec.hpp"
#endif

namespace BlazesRusCode
{
	//Designed to store trailing digits of number to reduce trunction loss(as replacement for float)
	//Based on https://medium.com/@JeffreyLean/the-secret-life-of-floating-point-types-in-programming-languages-e25bc55d6123
	//https://floating-point-gui.de/formats/fp/
	//and https://float.exposed
    class DLL_API AltFloat
    {
#if defined(AltFloat_DontUseTinyUDec)
		unsigned short Significant;
#else
		TinyUDec Significant;
#endif
		//If Exponent is zero,TinyUDec_IncludeFractionRepresentation not toggled, 
		// ,AltFloat_IncludeFractionalRep is toggled, and TinyUDec has negative DecimalHalf,
		// then treat AltFloat as a fraction
		short Exponent;
		
	#pragma region Addition Operations
        template<IntegerType IntType=int>
        void AdditionByIntOp(const IntType& rValue)
		{
		}
		
        void AdditionOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        template<IntegerType IntType=int>
        void SubtractionByIntOp(const IntType& rValue)
		{
		}
		
        void SubtractionOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Subtraction Operations
	
	#pragma region Multiplication Operations
        template<IntegerType IntType=int>
        void MultiplyByIntOp(const IntType& rValue)
		{
			//Multiplication by 10s can directly multiple Exponent by rValue
		}
		
        void MultiplyOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Multiplication Operations
	
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivideByIntOp(const IntType& rValue)
		{
			//Division by 10s can directly divide Exponent by rValue
		}
		
        void DivideOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Division Operations
	
	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModulusByIntOp(const IntType& rValue)
		{
		}
		
        void ModulusOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Modulus Operations
	}
}