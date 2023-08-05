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

#include <vector>

//Preprocessor options
/*
	BlazesFormulaCode_EnableOldElementFields
*/

namespace BlazesRusCode
{
	template<typename VarType>
    class DLL_API Formula
    {
		class DLL_API FormulaElement
		{
			//For Variables, it defines VariableName
			//For Numbers, it defines displayed value(updated every so often on formula change--Not important for AltNums)
			//For Functions, it displays function name(including ( part of function)
			//For FormulaEnd and FunctionEnd, DisplayVal == ')'
			//DisplayVal of ',' is ignored(used for Functions with more than 2 values)
			//DisplayVal of ' ' is ignored
			std::string DisplayVal;
		}
		class DLL_API VariableFormula
		{
		}
		class DLL_API NonOrganizedFormula : VariableFormula
		{
		}
		class DLL_API OrganizedFormula : VariableFormula
		{
			std::vector<NonOrganizedFormula> SubFormulas;
		}
	}
}