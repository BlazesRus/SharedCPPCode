// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\DLLAPI.h"

namespace BlazesRusCode
{
	/*
    // RepType: Enumerates the *current* representation type of a value.
    // Describes its canonical stored form after normalisation — not the
    // sequence of operations that produced it.
		//Layout is generated inside mixin
		RepTypeV3 bit layout order overview:
		

		Bits 0–3   : Core constant multipliers
								 (NormalType, PiNum, ENum, PiENum, INum)
								 → These identify the base constant family.
    Bits 4+
	*/
  template<class Policy, class RepTypeV3Layout>
  enum class RepTypeV3 : uint32_t {
		NormalType = 0,
		// top bit sentinel flag
    // Reserved: highest bit in uint32_t, used only as 'unknown' sentinel.
    // No real category will be assigned to this slot.
		UnknownType = (1u << 31);
		
		//Core multipliers
    PiNum    = Policy::EnablePi ? RepTypeV3Layout::PiNumRep : UnknownType,//(1u << 1)
    ENum    = Policy::EnableE ? RepTypeV3Layout::ENumRep : UnknownType,//(1u << 2)
    INum    = Policy::EnableI ? RepTypeV3Layout::INumRep : UnknownType,//(1u << 3)
		
		//Hybrid combinations
    PiENum    = Policy::EnablePi ? PiNum|ENum : UnknownType,
    PiINum    = Policy::EnablePiE ? PiNum|INum : UnknownType,
    EINum    = Policy::EnablePiE ? ENum|INum : UnknownType,
    PiEINum    = Policy::EnablePiE ? PiENum|INum : UnknownType,

    //Other math constants
		/*//Placeholders
    // Phi (φ) = (1 + sqrt(5)) / 2 ≈ 1.618...
    // The golden ratio, often used in geometry and number theory.
		// Special properties:
		//   - Reciprocal: 1/φ = φ - 1
		//   - Companion constant: sqrt(5) (φ is defined in terms of it)
		//   - Fibonacci link: F_n = (φ^n - (1-φ)^n)/sqrt(5)
		//   - closed form growth constant of the Fibonacci sequence
		// These relationships may justify treating φ, 1/φ, and sqrt(5)
		// as a coupled family of symbolic constants.
	  PhiNum    = Policy::EnablePhi ? RepTypeV3Layout::PhiNumRep : UnknownType,
		//√5
	  SqrtFiveNum    = Policy::EnablePhi ? RepTypeV3Layout::SqrtFiveNumRep : UnknownType,
		// PhiConjugateNum represents N × (1/φ).
		// Algebraic identity: 1/φ = φ − 1 (the conjugate of φ)
		PhiConjugateNum  = Policy::EnablePhi ? RepTypeV3Layout::DividedByPhiNumRep : UnknownType,
    //ln(2)
	  Ln2Num    = Policy::EnableLn2Num ? RepTypeV3Layout::Ln2Rep : UnknownType
    //Plastic constant(ρ) == ≈ 1.324717957
		//real root of (x^3 = x+1x^3 = x + 1)
		// PlasticSquareNum * PlasticNum is  reduced to (ρ + 1).
		//ρ is tied to Padovan and Perrin sequences
	  PlasticNum    = Policy::PlasticNum ? RepTypeV3Layout::PlasticNumRep : UnknownType
	  PlasticSquareNum    = Policy::PlasticNum ? RepTypeV3Layout::PlasticSquareNumRep : UnknownType
		*/
  };
	
}