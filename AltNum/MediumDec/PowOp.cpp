#include "MediumDec.hpp"
using MediumDecVariant = BlazesRusCode::MediumDec;

/// <summary>
/// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
/// </summary>
/// <param name="value">The target value.</param>
/// <param name="expNum">The numerator of the exponent value.</param>
/// <param name="expDenom">The denominator of the exponent value.</param>
inline auto BlazesRusCode::MediumDecVariant::FractionalPow(unsigned int expNum, unsigned int expDenom)
{
	auto targetVal = IntPowOp(expNum);
    auto CalcVal = MediumDecVariant::NthRoot(targetVal, expDenom);
    return CalcVal;
}

inline auto BlazesRusCode::MediumDecVariant::FractionalPow(boost::rational<int>& Frac)
{
	auto targetVal = IntPow(Frac.numerator());
	auto CalcVal = MediumDecVariant::NthRoot(targetVal, Frac.denominator());
	return CalcVal;
}

inline auto BlazesRusCode::MediumDecVariant::BasicPowOp(auto& expValue)
{
	boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
	switch (expValue.IntHalf)
	{
	case MirroredInt::Zero:
		return FractionalPow(Frac);
		break;
	case NegativeRep:
		return One / FractionalPow(Frac);
		break;
	default:
		if (expValue.IntHalf < 0){//Negative Exponent
			auto CalcVal = One / IntPow(expValue.IntHalf * -1);
			CalcVal /= FractionalPow(Frac);
			return CalcVal;
		} else {
			auto CalcVal = IntPowOp(expValue.IntHalf);
			CalcVal *= FractionalPow(Frac);
			return CalcVal;
		}
		break;
	}
}

/*
inline auto BlazesRusCode::MediumDecVariant::PowOp(const MediumDecVariant& exponent)
{
	if (exponent.DecimalHalf == 0)
		return IntPowOp(exponent.IntHalf);
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
                if(exponent.IntHalf==1)
                    return *this;
                else
                    return Zero;
                break;
    #if defined(AltNum_EnableIRep)
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
        		if (exponent.IntHalf < 0)
        			return Zero;
        		else if (IntHalf == -1 && exponent.GetIntegerPartition() % 2 == 0)
        			IntHalf = 1;
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
			auto targetVal = MediumDecVariant(exponent.IntHalf, exponent.DecimalHalf);
			targetVal = Pow(targetVal);
			return MediumDecVariant::NthRoot(targetVal, exponent.ExtraRep);
			break;
		}
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv:
		{
			auto targetVal = MediumDecVariant(exponent.IntHalf, exponent.DecimalHalf, PiRep);
			targetVal = Pow(targetVal);
			return MediumDecVariant::NthRoot(targetVal, -exponent.ExtraRep);
			break;
		}
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv:
		{
			auto targetVal = MediumDecVariant(exponent.IntHalf, exponent.DecimalHalf, ERep);
			targetVal = Pow(targetVal);
			return MediumDecVariant::NthRoot(targetVal, -expValue.ExtraRep);
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
			#if defined(AltNum_EnableApproaching)
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
		#if defined(AltNum_EnableApproaching)
		case RepType::ApproachingBottom:
			if(exponent.IntHalf==0)
				return ApproachingOneFromRightValue();
			else if(exponent.IntHalf==NegativeRep)
				return ApproachingOneFromLeftValue();
			else
				//exponent.ConvertToNormType(expType);
			break;
		#endif
		#if defined(AltNum_EnableIRep)
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
	auto expValue = exponent;//Copy the value to allow converting
    expValue.ConvertToNormType(expType);
	boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
	switch (expValue.IntHalf)
	{
		case 0:
			return FractionalPow(Frac);
			break;
		case MediumDecVariant::NegativeRep:
			return One / FractionalPow(Frac);
			break;
		default:
			if (expValue.IntHalf < 0)//Negative Exponent 
			{
				auto CalcVal = One / IntPow(expValue.IntHalf * -1);
				CalcVal /= FractionalPow(Frac);
				return CalcVal;
			}
			else
			{
				auto CalcVal = IntPowOp(expValue.IntHalf);
				CalcVal *= FractionalPow(Frac);
				return CalcVal;
			}
			break;
	}
}
*/