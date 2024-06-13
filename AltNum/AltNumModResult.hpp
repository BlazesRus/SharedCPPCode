// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

namespace BlazesRusCode
{
    /// <summary>
    /// Performs remainder/Mod operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    template<MediumDecVariant VarType>
    class AltNumModResult
    {//based on https://embeddedgurus.com/stack-overflow/2011/02/efficient-c-tip-13-use-the-modulus-operator-with-caution/
    public:
		//Division result
        VarType divRes;

		//Returns result from modulus
        VarType modRes;

		auto GetResult()
		{
			return modRes;
		}

		auto ReturnDivisionResult()
		{
			return divRes;
		}

		/// <summary>
        /// Initializes a new instance of the <see cref="AltNumModResult"/> class.
		/// Convert down to base class before passing to perimeter to bypass incomplete class issues
        /// </summary>
        /// <param name="lValue">The left side variable to perform modulus on</param>
        /// <param name="rValue">The right side variable</param>
		AltNumModResult(const VarType& lValue, const VarType& rValue)
		{
            divRes = lValue / rValue;
            modRes = lValue - rValue * divRes;
		}

        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        friend AltNumModResult operator%(const VarType& lValue, const VarType& rValue)
		{
            divRes = lValue / rValue;
            modRes = lValue - rValue * divRes;
			return *this;
		}

    #pragma region ConvertToOtherTypes

        /// <summary>
        /// Calculates if remainder is zero. (explicit conversion operation)
        /// </summary>
        /// <returns>True if the remainder is equal to zero</returns>
        explicit operator bool(){ return modRes==0; }

        /// <summary>
        /// Returns modulus operation result value (explicit conversion operation)
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator VarType(){ return modRes; }

        /// <summary>
        /// Unary + Operator (Returns modulus operation result value)
		/// The unary plus (+) operator precedes its operand and evaluates to its operand but attempts to convert it into a number, if it isn't already.
        /// </summary>
        /// <param name="self">The self.</param>
        VarType operator+ () const
        {
			return modRes;
        }

    #pragma endregion ConvertToOtherTypes

    };
}