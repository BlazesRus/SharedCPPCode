// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

namespace BlazesRusCode
{


	//Designed to store trailing digits of number to reduce trunction loss(as replacement for float)
	//Based on https://medium.com/@JeffreyLean/the-secret-life-of-floating-point-types-in-programming-languages-e25bc55d6123
	//and https://float.exposed
    class DLL_API AltFloat
    {
		short Exponent;
		short Significant;
	}
}