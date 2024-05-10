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
    template<typename VarType>
    class AltNumModResult
    {//based on https://embeddedgurus.com/stack-overflow/2011/02/efficient-c-tip-13-use-the-modulus-operator-with-caution/
    public:
		//Division result
        VarType divRes = VarType::Zero;
		//Returns result from modulus
        VarType modRes = VarType::Zero;
		
		VarType GetResult()
		{
			return modRes;
		}
		
		VarType ReturnDivisionResult()
		{
			return divResult;
		}
		
		/// <summary>
        /// Initializes a new instance of the <see cref="AltNumModResult"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
		AltNumModResult(VarType LValue, VarType RValue)
		{
            divRes = LValue / RValue;
            modRes = LValue - RValue * divRes;
		}

        AltNumModResult(const MediumDecBase&) = default;

    #pragma region ConvertToOtherTypes
	
        /// <summary>
        /// Calculates if remainder is zero. (implicit conversion operation)
        /// </summary>
        /// <returns>True if the remainder is equal to zero</returns>
        implicit operator bool(){ return modRes==0; }
		
        /// <summary>
        /// Calculates if remainder is zero.
        /// </summary>
        /// <returns>True if the remainder is equal to zero</returns>
		bool operator! () 
		{
			return modRes==0;
		} const
		
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
        /// <returns>MediumDecBase</returns>
        VarType operator+ ()
        {
			return modRes;
        } const
	
    #pragma endregion ConvertToOtherTypes
	
        /// <summary>
        /// Modulus Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend AltNumModResult operator%(const VarType& LValue, const VarType& RValue)
		{
            divRes = LValue / RValue;
            modRes = LValue - RValue * divRes;
			return *this;
		}
	
    };
}