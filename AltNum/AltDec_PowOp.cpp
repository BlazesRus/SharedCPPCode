#include "AltDec.hpp"

/// <summary>
/// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
/// </summary>
/// <param name="value">The target value.</param>
/// <param name="expNum">The numerator of the exponent value.</param>
/// <param name="expDenom">The denominator of the exponent value.</param>
inline AltDec BlazesRusCode::AltDec::FractionalPow(AltDec value, int expNum, int expDenom)
{
    AltDec CalcVal = AltDec::NthRoot(AltDec::Pow(value, expNum), expDenom);
    return CalcVal;
}

inline AltDec BlazesRusCode::AltDec::FractionalPow(AltDec& value, boost::rational<int>& Frac)
{
	AltDec CalcVal = AltDec::NthRoot(AltDec::Pow(value, Frac.numerator()), Frac.denominator());
	return CalcVal;
}

inline void BlazesRusCode::AltDec::BasicPowOp(AltDec& expValue)
{
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, AltDec::DecimalOverflow);
	switch (expValue.IntValue)
	{
	case 0:
		return FractionalPow(&this, Frac);
		break;
	case AltDec::NegativeRep:
		return 1 / FractionalPow(&this, Frac);
		break;
	default:
	{
		if (expValue.IntValue < 0)//Negative Exponent 
		{
			AltDec CalcVal = 1 / Pow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(&this, Frac);
			return CalcVal;
		}
		else
		{
			AltDec CalcVal = Pow(expValue.IntValue);
			CalcVal *= FractionalPow(&this, Frac);
			return CalcVal;
		}
		break;
	}
	}
}

inline AltDec BlazesRusCode::AltDec::PowOp(AltDec& expValue)
{
	if (expValue.DecimalHalf == 0)
		return Pow(expValue.IntValue);
	else if (DecimalHalf == InfinityRep)
	{
		if (expValue == Zero)
#if defined(AltNum_EnableNaN)
			SetAsUndefined();
#else
			throw "Infinity to power of Zero returns Undefined value";
#endif
		else if (expValue.IntValue.Value < 0)
			SetAsZero();
		else if (IntValue.Value == -1 && expValue.IntValue.Value % 2 == 0)
			IntValue.Value = 1;
		else
			return;//Returns infinity
		return;
	}
#if defined(AltNum_EnableNaN)
#endif
	RepType expType = expValue.GetRepType();
	switch (expType)
	{
#if defined(AltNum_EnableFractionals)
	case RepType::NumByDiv:
		return AltDec::NthRoot(AltDec::Pow(value, AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#endif
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
	case RepType::PiNumByDiv:
		ConvertPiByDivToNumByDiv();
		return AltDec::NthRoot(AltDec::Pow(value, AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#elif defined(AltNum_EnablePiFractional)
	case RepType::PiFractional:
		AltDec NumExp = AltDec(expValue.IntValue, expValue.DecimalHalf);
		NumExp *= PiNum;
		return AltDec::NthRoot(AltDec::Pow(value, NumExp), -expValue.ExtraRep);
		break;
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::ENumByDiv:
		ConvertEByDivToNumByDiv();
		return AltDec::NthRoot(AltDec::Pow(value, AltDec(expValue.IntValue, expValue.DecimalHalf)), expValue.ExtraRep);
		break;
#elif defined(AltNum_EnableEFractional)
	case RepType::EFractional:
		AltDec NumExp = AltDec(expValue.IntValue, expValue.DecimalHalf);
		NumExp *= ENum;
		return AltDec::NthRoot(AltDec::Pow(value, NumExp), -expValue.ExtraRep);
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
		if (value < 0)
			value.SetAsNegativeInfinity();
		else
			value.SetAsInfinity();//Techically within range of Positive and NegativeInfinity
		return;
		break;
	case RepType::NegativeInfinity:
#if defined(AltNum_EnableApproachingValues)
		if (value < 0)
			value.SetAsApproachingBottom(NegativeRep);
		else
			value.SetAsApproachingBottom();
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
		expValue.ConvertToNormType(LRep);
		break;
	}
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, AltDec::DecimalOverflow);
	switch (expValue.IntValue)
	{
	case 0:
		return FractionalPow(value, Frac);
		break;
	case AltDec::NegativeRep:
		return 1 / FractionalPow(value, Frac);
		break;
	default:
	{
		if (expValue.IntValue < 0)//Negative Exponent 
		{
			AltDec CalcVal = 1 / Pow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(value, Frac);
			return CalcVal;
		}
		else
		{
			AltDec CalcVal = Pow(expValue.IntValue);
			CalcVal *= FractionalPow(value, Frac);
			return CalcVal;
		}
		break;
	}
	}
}