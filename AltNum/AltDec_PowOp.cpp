#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

/// <summary>
/// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
/// </summary>
/// <param name="value">The target value.</param>
/// <param name="expNum">The numerator of the exponent value.</param>
/// <param name="expDenom">The denominator of the exponent value.</param>
inline AltDec BlazesRusCode::AltDec::FractionalPow(int expNum, int expDenom)
{
    AltDec CalcVal = AltDec::NthRoot(Int32PowOp(expNum), expDenom);
    return CalcVal;
}

inline AltDec BlazesRusCode::AltDec::FractionalPow(boost::rational<int>& Frac)
{
	AltDec CalcVal = AltDec::NthRoot(Int32Pow(Frac.numerator()), Frac.denominator());
	return CalcVal;
}

inline AltDec BlazesRusCode::AltDec::BasicPowOp(AltDec& expValue)
{
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, AltDec::DecimalOverflow);
	switch (expValue.IntValue)
	{
	case 0:
		return FractionalPow(Frac);
		break;
	case AltDec::NegativeRep:
		return One / FractionalPow(Frac);
		break;
	default:
	{
		if (expValue.IntValue < 0)//Negative Exponent 
		{
			AltDec CalcVal = One / Int32Pow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(Frac);
			return CalcVal;
		}
		else
		{
			AltDec CalcVal = Int32PowOp(expValue.IntValue);
			CalcVal *= FractionalPow(Frac);
			return CalcVal;
		}
		break;
	}
	}
}

inline AltDec BlazesRusCode::AltDec::PowOp(AltDec& expValue)
{
	if (expValue.DecimalHalf == 0)
		return Int32PowOp(expValue.IntValue);
	else if (DecimalHalf == InfinityRep)
	{
		if (expValue == Zero)
#if defined(AltNum_EnableNaN)
			SetAsUndefined();
#else
			throw "Infinity to power of Zero returns Undefined value";
#endif
		else if (expValue.IntValue < 0)
			return Zero;
		else if (IntValue == -1 && expValue.GetIntHalf() % 2 == 0)
			IntValue = 1;
		else
			return *this;//Returns infinity
		return;
	}
#if defined(AltNum_EnableNaN)
#endif
	RepType expType = expValue.GetRepType();
	switch (expType)
	{
#if defined(AltNum_EnableFractionals)
	case RepType::NumByDiv:
		return AltDec::NthRoot(Pow(AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#endif
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
	case RepType::PiNumByDiv:
		ConvertPiByDivToNumByDiv();
		return AltDec::NthRoot(Pow(AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#elif defined(AltNum_EnablePiFractional)
	case RepType::PiFractional:
		AltDec NumExp = AltDec(expValue.IntValue, expValue.DecimalHalf);
		NumExp *= PiNum;
		return AltDec::NthRoot(Pow(NumExp), -expValue.ExtraRep);
		break;
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::ENumByDiv:
		ConvertEByDivToNumByDiv();
		return AltDec::NthRoot(Pow(AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#elif defined(AltNum_EnableEFractional)
	case RepType::EFractional:
		AltDec NumExp = AltDec(expValue.IntValue, expValue.DecimalHalf);
		NumExp *= ENum;
		return AltDec::NthRoot(Pow(NumExp), -expValue.ExtraRep);
		break;
#endif
#endif
	#if defined(AltNum_EnableInfinityRep)
		//0^Infinity = 0
		//2^PositiveInfinity = PositiveInfinity
		//-2^PositiveInfinity = Within range of Positive and NegativeInfinity(undefined?)
		//2^NegativeInfinity = ApproachingZero
		//-2^NegativeInfinity = -Approaching Zero
	case RepType::PositiveInfinity:
		if (IsNegative())
			return NegativeInfinity;
		else
			return Infinity;//Techically within range of Positive and NegativeInfinity
		return;
		break;
	case RepType::NegativeInfinity:
#if defined(AltNum_EnableApproachingValues)
		if (IsNegative())
			SetAsApproachingBottom(NegativeRep);
		else
			SetAsApproachingBottom();
#else
		value.SetAsZero();
#endif
		return;
		break;
	#endif
#if defined(AltNum_EnableImaginaryNum)
	case RepType::INum:
#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::INumByDiv:
#elif defined(AltNum_EnableEFractional)
	case RepType::IFractional:
#endif
#if defined(AltNum_EnableImaginaryInfinity)
	case RepType::PositiveImaginaryInfinity:
	case RepType::NegativeImaginaryInfinity:
#if defined(AltNum_EnableApproachingI)
	case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
#if !defined(AltNum_DisableApproachingTop)
	case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
#endif
#if defined(AltNum_EnableApproachingDivided)
	case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
#if !defined(AltNum_DisableApproachingTop)
	case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
#endif
#endif
#endif
#endif
		throw "operation to power of imaginary number not supported";
		break;
#endif
#if defined(AltNum_EnableNaN)
	case RepType::Undefined:
	case RepType::NaN:
		throw "Can't perform operations with NaN or Undefined number";
		break;
#endif			
	default:
		expValue.ConvertToNormType(expType);
		break;
	}
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, AltDec::DecimalOverflow);
	switch (expValue.IntValue)
	{
	case 0:
		return FractionalPow(Frac);
		break;
	case AltDec::NegativeRep:
		return One / FractionalPow(Frac);
		break;
	default:
	{
		if (expValue.IntValue < 0)//Negative Exponent 
		{
			AltDec CalcVal = One / Int32Pow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(Frac);
			return CalcVal;
		}
		else
		{
			AltDec CalcVal = Int32PowOp(expValue.IntValue);
			CalcVal *= FractionalPow(Frac);
			return CalcVal;
		}
		break;
	}
	}
}