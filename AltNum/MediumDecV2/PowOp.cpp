#include "MediumDecV2Base.hpp"
using MediumDecV2Base = BlazesRusCode::MediumDecV2Base;

/// <summary>
/// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
/// </summary>
/// <param name="value">The target value.</param>
/// <param name="expNum">The numerator of the exponent value.</param>
/// <param name="expDenom">The denominator of the exponent value.</param>
inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::FractionalPow(int expNum, int expDenom)
{
	MediumDecV2Base targetVal = IntPowOp(expNum);
    MediumDecV2Base CalcVal = MediumDecV2Base::NthRoot(targetVal, expDenom);
    return CalcVal;
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::FractionalPow(boost::rational<int>& Frac)
{
	MediumDecV2Base targetVal = IntPow(Frac.numerator());
	MediumDecV2Base CalcVal = MediumDecV2Base::NthRoot(targetVal, Frac.denominator());
	return CalcVal;
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::BasicPowOp(MediumDecV2Base& expValue)
{
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, MediumDecV2Base::DecimalOverflow);
	switch (expValue.IntValue)
	{
	case 0:
		return FractionalPow(Frac);
		break;
	case MediumDecV2Base::NegativeRep:
		return One / FractionalPow(Frac);
		break;
	default:
	{
		if (expValue.IntValue < 0)//Negative Exponent 
		{
			MediumDecV2Base CalcVal = One / IntPow(expValue.IntValue * -1);
			CalcVal /= FractionalPow(Frac);
			return CalcVal;
		}
		else
		{
			MediumDecV2Base CalcVal = IntPowOp(expValue.IntValue);
			CalcVal *= FractionalPow(Frac);
			return CalcVal;
		}
		break;
	}
	}
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::PowOp(MediumDecV2Base& expValue)
{
	if (expValue.DecimalHalf == 0)
		return IntPowOp(expValue.IntValue);
#if defined(AltNum_EnableInfinityRep)
	else if (DecimalHalf == InfinityRep)
	{
		if (expValue == Zero)
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
                if(expValue.IntValue==1)
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
            	RepType expType = expValue.GetRepType();
        		if (expValue.IntValue < 0)
        			return Zero;
        		else if (IntValue == -1 && expValue.GetIntHalf() % 2 == 0)
        			IntValue = 1;
        		else
        			return *this;//Returns infinity
        }
        return *this;
	}
#endif
	RepType expType = expValue.GetRepType();
	switch (expType)
	{
		#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
		{
			MediumDecV2Base targetVal = MediumDecV2Base(expValue.IntValue, expValue.DecimalHalf);
			targetVal = Pow(targetVal);
			return MediumDecV2Base::NthRoot(targetVal, expValue.ExtraRep);
			break;
		}
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv:
		{
			MediumDecV2Base targetVal = MediumDecV2Base(expValue.IntValue, expValue.DecimalHalf, PiRep);
			targetVal = Pow(targetVal);
			return MediumDecV2Base::NthRoot(targetVal, -expValue.ExtraRep);
			break;
		}
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv:
		{
			MediumDecV2Base targetVal = MediumDecV2Base(expValue.IntValue, expValue.DecimalHalf, ERep);
			targetVal = Pow(targetVal);
			return MediumDecV2Base::NthRoot(targetVal, -expValue.ExtraRep);
			break;
		}
		#endif
		#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional:
		{
			MediumDecV2Base targetVal = MediumDecV2Base(expValue.IntValue, 0, PiRep);
			targetVal = Pow(targetVal);
			return MediumDecV2Base::NthRoot(targetVal, expValue.DecimalHalf);
		}
		#endif
		#if defined(AltNum_EnableEFractional)
		case RepType::EFractional:
		{
			MediumDecV2Base targetVal = MediumDecV2Base(expValue.IntValue, 0, ERep);
			targetVal = Pow(targetVal);
			return MediumDecV2Base::NthRoot(targetVal, expValue.DecimalHalf);
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
				return Infinity;//Techically within range of Positive and NegativeInfinity
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
			if(expValue.IntValue==0)
				return ApproachingOneFromRightValue();
			else if(expValue.IntValue==NegativeRep)
				return ApproachingOneFromLeftValue();
			else
				expValue.ConvertToNormType(expType);
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
			expValue.ConvertToNormType(expType);
			break;
	}
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, MediumDecV2Base::DecimalOverflow);
	switch (expValue.IntValue)
	{
		case 0:
			return FractionalPow(Frac);
			break;
		case MediumDecV2Base::NegativeRep:
			return One / FractionalPow(Frac);
			break;
		default:
			if (expValue.IntValue < 0)//Negative Exponent 
			{
				MediumDecV2Base CalcVal = One / IntPow(expValue.IntValue * -1);
				CalcVal /= FractionalPow(Frac);
				return CalcVal;
			}
			else
			{
				MediumDecV2Base CalcVal = IntPowOp(expValue.IntValue);
				CalcVal *= FractionalPow(Frac);
				return CalcVal;
			}
			break;
	}
}
