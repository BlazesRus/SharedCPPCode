#include "MediumDecV2Base.hpp"
using MediumDecVariant = BlazesRusCode::MediumDecV2Base;

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

inline auto BlazesRusCode::MediumDecVariant::BasicPowOp(const auto& expValue)
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

inline auto BlazesRusCode::MediumDecVariant::PowOp(const auto& exponent)
{
	if (exponent.DecimalHalf == 0)
		return IntPowOp(exponent.IntHalf);
    else if(IsOne()){
        SetAsOne(); return *this; 
    } else if(exponent.DecimalHalf.Flags==3)
        throw "Needs complex number support to give result of power of i";
#if defined(AltNum_EnableInfinityRep)
	else if (DecimalHalf == InfinityRep){
        if(exponent.IsZero())
    #if defined(AltNum_EnableNaN)
			return Undefined;
    #else
			throw "Infinity to power of Zero returns Undefined value";
    #endif
        else
            
    }
    else if(exponent.DecimalHalf == InfinityRep){
       if(exponent.IsPositive())
           return *this;
       else
        #if defined(AltNum_EnableApproaching)
            SetAsApproachingBottomVal();
        #else
            SetAsZero();
        #endif
        return *this;
    }
#endif
    } else if(IsNegOne()){
        SetAsOne(); 
        return *this; 
    }
    RepType expType = exponent.GetRepType();
	switch (expType)
	{
	#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
       #if defined(AltNum_EnablePiRep)
	        case RepType::PiNumByDiv:
       #endif
       #if defined(AltNum_EnableERep)
	        case RepType::ENumByDiv:
       #endif
			auto targetVal = AltDecBase(exponent.IntHalf, exponent.DecimalHalf);
			targetVal = Pow(targetVal);
			AltDecBase::NthRootOf(exponent.ExtraRep);
            return *this;
            break;
    #endif
    #if defined(AltNum_EnableApproaching)
		case RepType::ApproachingBottom:
            if(exponent.IntHalf==0){
                IntHalf.Value = 1;
                DecimalHalf == ApproachingBottomRep;
                return *this;
            }
            break;
		case RepType::ApproachingTop:
            if(exponent.IntHalf==0&&DecimalHalf.Value==0){
                --IntHalf.Value;
                DecimalHalf.Value == ApproachingTopRep;
                return *this;
            }
            break;
    #endif
    }
	//If don't return value before this point convert into normal Type
	auto expValue = exponent;//Copy the value to allow converting
    expValue.ConvertToNormType(expType);
    BasicPowOp(expValue);
    return *this;
}