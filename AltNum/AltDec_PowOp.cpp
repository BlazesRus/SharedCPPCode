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
	AltDec targetVal = IntPowOp(expNum);
    AltDec CalcVal = AltDec::NthRoot(targetVal, expDenom);
    return CalcVal;
}

inline AltDec BlazesRusCode::AltDec::FractionalPow(boost::rational<int>& Frac)
{
	AltDec targetVal = IntPow(Frac.numerator());
	AltDec CalcVal = AltDec::NthRoot(targetVal, Frac.denominator());
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
			AltDec CalcVal = One / IntPow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(Frac);
			return CalcVal;
		}
		else
		{
			AltDec CalcVal = IntPowOp(expValue.IntValue);
			CalcVal *= FractionalPow(Frac);
			return CalcVal;
		}
		break;
	}
	}
}

inline AltDec BlazesRusCode::AltDec::PowOp(const AltDec& exponent)
{
	if (exponent.DecimalHalf == 0)
		return IntPowOp(exponent.IntValue);
#if defined(AltNum_EnableInfinityRep)
	else if (DecimalHalf == InfinityRep)
	{
		if (exponent == Zero)
    #if defined(AltNum_EnableNaN)
			return Undefined;
    #else
			throw "Infinity to power of Zero returns Undefined value";
    #endif
	    RepType RRep = GetRepType();
        switch(RRep)
        {
	        case RepType::PositiveInfinity:
	        case RepType::NegativeInfinity:
                if(exponent.IntValue==1)
                    return *this;
                else
                    return Zero;
                break;
    #if defined(AltNum_EnableImaginaryNum)
	        case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
	        case RepType::INumByDiv:
        #elif defined(AltNum_EnableIFractional)
	        case RepType::IFractional:
        #endif
        #if defined(AltNum_EnableNaN)
			    return Undefined;
        #else
			    throw "Infinity to power of i returns Undefined value";
        #endif
    #endif
            default:
            	RepType expType = exponent.GetRepType();
        		if (exponent.IntValue < 0)
        			return Zero;
        		else if (IntValue == -1 && exponent.GetIntHalf() % 2 == 0)
        			IntValue = 1;
        		else
        			return *this;//Returns infinity
        }
        return *this;
	}
#endif
	RepType expType = exponent.GetRepType();
	switch (expType)
	{
		#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
		{
			AltDec targetVal = AltDec(exponent.IntValue, exponent.DecimalHalf);
			targetVal = Pow(targetVal);
			return AltDec::NthRoot(targetVal, exponent.ExtraRep);
			break;
		}
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv:
		{
			AltDec targetVal = AltDec(exponent.IntValue, exponent.DecimalHalf, PiRep);
			targetVal = Pow(targetVal);
			return AltDec::NthRoot(targetVal, -exponent.ExtraRep);
			break;
		}
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv:
		{
			AltDec targetVal = AltDec(exponent.IntValue, exponent.DecimalHalf, ERep);
			targetVal = Pow(targetVal);
			return AltDec::NthRoot(targetVal, -expValue.ExtraRep);
			break;
		}
		#endif
		#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional:
		{
			AltDec targetVal = AltDec(exponent.IntValue, 0, PiRep);
			targetVal = Pow(targetVal);
			return AltDec::NthRoot(targetVal, expValue.DecimalHalf);
		}
		#endif
		#if defined(AltNum_EnableEFractional)
		case RepType::EFractional:
		{
			AltDec targetVal = AltDec(exponent.IntValue, 0, ERep);
			targetVal = Pow(targetVal);
			return AltDec::NthRoot(targetVal, exponent.DecimalHalf);
			break;
		}
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
				return Infinity;//Technically within range of Positive and NegativeInfinity
			return *this;
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
			return *this;
			break;
		#endif
		#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			if(exponent.IntValue==0)
				return ApproachingOneFromRightValue();
			else if(exponent.IntValue==NegativeRep)
				return ApproachingOneFromLeftValue();
			else
				//exponent.ConvertToNormType(expType);
			break;
		#endif
		#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv:
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional:
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
		case RepType::PositiveImaginaryInfinity:
		case RepType::NegativeImaginaryInfinity:
		#endif
		#if defined(AltNum_EnableApproachingI)
		case RepType::ApproachingImaginaryBottom:
			#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingImaginaryTop:
			#endif
			#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingImaginaryMidLeft:
				#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingImaginaryMidRight:
				#endif
			#endif
		#endif
			throw "Needs complex number support to give result of power of i";
			break;
		#endif
		default:
			//expValue.ConvertToNormType(expType);
			break;
	}
	//If don't return value before this point convert into normal Type
	AltDec expValue = exponent;//Copy the value to allow converting
    expValue.ConvertToNormType(expType);
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
			if (expValue.IntValue < 0)//Negative Exponent 
			{
				AltDec CalcVal = One / IntPow(expValue.IntValue * -1);
				CalcVal /= FractionalPow(Frac);
				return CalcVal;
			}
			else
			{
				AltDec CalcVal = IntPowOp(expValue.IntValue);
				CalcVal *= FractionalPow(Frac);
				return CalcVal;
			}
			break;
	}
}
