#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::RepType;

inline void AltDec::DivOpSameRep_CatchAll(const AltDec& rValue, const RepType& LRep)
{
    ConvertFromAbtract(LRep);
	auto RValue = rValue.ConvertAsNormType(LRep);
	lValue.BasicUnsignedDivOp(RValue);
}

inline void AltDec::DivOpSameRep_CatchAllV2(const AltDec& rValue, const RepType& LRep)
{
    ConvertFromAbtract(LRep);
	auto RValue = rValue.ConvertToNormTypeFromOther(LRep);
	BasicUnsignedDivOp(RValue);
}

inline void AltDec::DivOpSameRep_ApproachingBottom(const AltDec& rValue)
{
    if(lValue.IntHalf.Value==0)//0.0..01/2.0..01 = ~0.0..01
		return;
	else if(rValue.IntHalf.Value==0)//2.0..01/0.0..01 = infinity
#if defined(AltNum_EnableInfinity)
		SetAsInfinityVal();
#else
		DivOpSameRep_CatchAll(rValue, RepType::ApproachingBottom);
#endif
	else
		DivOpSameRep_CatchAll(rValue, RepType::ApproachingBottom);
}

inline void AltDec::DivOpSameRep_ApproachingTop(const AltDec& rValue)
{
#if defined(AltNum_EnableApproachingDivided)
	//0.9..9/2.9..9 = ~0.9..9/3 (not quite that but close enough)
    if(IntHalf.Value==0){
		DecimalHalf.Value = ApproachingMidLeftRep;
		ExtraRep = rValue.IntHalf+1;
	} else if(rValue.IntHalf.Value==0){
		DecimalHalf.Value = ApproachingBottomRep;
		++IntHalf.Value;
	}
	//999999999999999.999999999999999/0.999 999 999 999 999 = 1000000000000001
	//9.999 999 999 999 999/0.999 999 999 999 999 = 10.000000000000009000000000000009000000000000009000000000000009000000000000009000000000000009
	//5.99999999999999999999999999999999999999999999/0.99999999999999999999999999999999999999999999 = 6.0000000000000000000000000000000000000000000500000000000000000000000000000000000000000005
	else
		CatchAllDivisionV2(rValue, LRep);
#else
	DivOpSameRep_CatchAll(rValue, RepType::ApproachingTop);
#endif
}

#if defined(AltNum_EnableFractionals)
void SameRep_NumByDiv(const auto& rValue, const RepType& LRep)
{
/*	//((AltDecBase(IntHalf,DecimalHalf))/ExtraRep) / (AltDecBase(rValue.IntHalf,rValue.DecimalHalf))/rValue.ExtraRep) = 
	//((AltDecBase(IntHalf,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(AltDecBase(rValue.IntHalf,rValue.DecimalHalf)))
	if (rValue < 0)
	{
		rValue *= -1;
		SwapNegativeStatus();
	}
	if (rValue.DecimalHalf == 0)
	{
		BasicUIntMultOp(rValue.ExtraRep);
		int result = ExtraRep * rValue.IntHalf;
		if(ExtraRep == result / rValue.IntHalf)//checking for overflow
		{
			ExtraRep = result;
		}
		else
			BasicUnsignedDivOp(rValue.IntHalf);
	}
	else
	{//CatchAllDivisionV2(rValue, LRep);
		BasicUIntMultOp(rValue.ExtraRep);
		BasicUnsignedDivOp(rValue);
	}
*/    
	//Add Code Here
}
#endif

#if defined(AltNum_EnablePowerOfRepresentation)
void SameRep_PowerOf(const auto& rValue, const RepType& LRep)
{
	if(IntHalf==rValue.IntHalf&&DecimalHalf==rValue.DecimalHalf){//(1.5Pi^4)/(1.5Pi^2)=(1.5Pi^2)
	#if defined(AltNum_EnableNegativePowerRep)
		ExtraRep -= rValue.ExtraRep;
	#else
		if(ExtraRep.Value>rValue.ExtraRep)
			ExtraRep.Value -= rValue.ExtraRep;
		else {//Result is negative Exponent
			signed int Exp = (signed int)ExtraRep.Value - (signed int)rValue.ExtraRep.Value;
			ResetDivisor();
			BasicIntPowOpV1(Exp);
		}
	#endif
	}
	else
		CatchAllOp(rValue, LRep, LRep);
}
#endif

void SameRep_MixedFrac(const auto& rValue, const RepType& LRep)
{
/*	int rvDivisor = -rValue.ExtraRep;
	//=LeftSideNum*rValue.ExtraRep / RightSideNum;
	AltDecBase LeftSideNum;
	if (IntHalf == NegativeRep)
		LeftSideNum = AltDecBase(DecimalHalf);
	else if (IntHalf < 0)
		LeftSideNum = AltDecBase(IntHalf * ExtraRep + DecimalHalf);
	else if (IntHalf == 0)
		LeftSideNum = AltDecBase(-DecimalHalf);
	else
		LeftSideNum = AltDecBase(IntHalf * ExtraRep - DecimalHalf);
	LeftSideNum.UIntDivOp(rValue.ExtraRep);
	if (LeftSideNum.IsZero())
		SetAsZero();
	else
	{
		DecimalHalf = LeftSideNum.DecimalHalf;
		if(rValue<0)
		{
			IntHalf = -LeftSideNum.IntHalf;
			ExtraRep *= rValue.IntHalf==NegativeRep ? -rValue.DecimalHalf : -rValue.IntHalf * rValue.ExtraRep - rValue.DecimalHalf;
		}
		else
		{
			IntHalf = LeftSideNum.IntHalf;
			ExtraRep *= rValue.IntHalf==0 ? -rValue.DecimalHalf : rValue.IntHalf * rValue.ExtraRep - rValue.DecimalHalf;
		}
	}*/
    //Add Code Here
}

void AltDec::UnsignedDivOp(const AltDec& rValue)
{
	#if defined(AltNum_EnableInfinityRep)
	if(DecimalHalf.Value==InfinityRep){
		if(rValue.DecimalHalf.Value==InfinityRep)
		{
		#if defined(AltNum_EnableIRep)
			if(rValue.DecimalHalf.Flags==3) {//Divided by Imaginary Infinity
				if(DecimalHalf.Flags==3)
					DecimalHalf.Flags = 0
				else {
					SwapNegativeStatus();
					DecimalHalf.Flags = 3;
				}
			}
		#endif
		#if defined(AltNum_EnableInfinityPowers)
			if(IntHalf.Value==1)
				SetAsZero();
			else
				--IntHalf.Value;
		#elif defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(InfDividedByInfRep);
		#else
			throw "Can't divide infinity by infinity";
		#endif
		} else if(rValue.IsZero())
		#if defined(AltNum_EnableInfinityPowers)
			++IntHalf.Value;//Make into even larger infinity
		#elif defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(InfDividedByZeroRep);
		#elif defined(AltNum_DefineDivideByZeroAsInfinity)
			return *this;
		#elif defined(AltNum_EnableNaN)
            DecimalHalf = NaNRep;
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
		return *this;
	} else if(rValue.DecimalHalf.Value==InfinityRep){//Divided by Infinity
        DecimalHalf = ApproachingBottomRep;
		IntHalf.Value = 0;
        ExtraRep = InitialExtraRep;
		if(rValue.DecimalHalf.Flags==3){//Divided by Imaginary infinity
			SwapNegativeStatus();
			DecimalHalf.Flags = 3;
		}
		return *this;
	}
	#endif
	if(IsZero()){
		if(rValue.IsZero())
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(DividedByZeroRep);
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
		else
			return;
	} else if(rValue.IsZero()){
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(DividedByZeroRep);
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
	} else if(DecimalHalf<=NaNVariantThreshold&&IntHalf==rValue.IntHalf&&DecimalHalf.Value==rValue.DecimalHalf.Value)
		SetAsOneVal();
	else {
		RepType LRep = GetNormRepType();
		RepType RRep = rValue.GetNormRepType();
		if(LRep==RRep)
		{
			switch(LRep)
			{
			case RepTypeEnum::NormalType:
				BasicUnsignedDivOp(rValue);
				break;
			break;

#if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingBottom:
				DivOpSameRep_ApproachingBottom(rValue);
				break;
			case RepTypeEnum::ApproachingTop:
				DivOpSameRep_ApproachingTop(rValue);
				break;
#endif
#if defined(AltDec_EnableUndefinedButInRange)
			case RepType::UndefinedButInRange:
				throw "Operation not supported at moment."; break;
#endif
			default:
				throw "Operation not supported at moment."; break;
			}
		}
		else
		{
			switch(LRep)
			{
				case RepTypeEnum::NormalType:
				{
					switch(RRep)
					{
						default:
							BasicUnsignedDivOp(rValue.ConvertAsNormType(RRep));
							break;
					}
				} break;
	#if defined(AltNum_EnableApproaching)
				case RepTypeEnum::ApproachingBottom:
				{
					switch(RRep)
					{
						case RepTypeEnum::NormalType:
							if(IntHalf.Value!=0){
								ConvertToNormType(LRep);
								BasicUnsignedDivOp(rValue);
							}
							break;
						default:
							ConvertToNormType(LRep);
							BasicUnsignedDivOp(rValue.ConvertAsNormType(RRep));
							break;
					}
				} break;
				case RepTypeEnum::ApproachingTop:
				{
					switch(RRep)
					{
						case RepTypeEnum::NormalType:
							if(IntHalf.Value!=0){
								ConvertToNormType(LRep);
								BasicUnsignedDivOp(rValue);
							}
							break;
						default:
							ConvertToNormType(LRep);
							BasicUnsignedDivOp(rValue.ConvertAsNormType(RRep));
							break;
					}
				} break;
	#endif
				default:
					throw "Operation not supported"; break;
			}
		}
		if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
		{
			if(DecimalHalf.Flags!=0)
				DecimalHalf.Flags = 0;
        }
        else
        {
            switch(rValue.DecimalHalf.Flags)
            {
            	case 1:
            		BasicUnsignedDivOp(PiNum); break;
            	case 2:
            		BasicUnsignedDivOp(ENum); break;
            	case 3:
            		SwapNegativeStatus();
					if(GetFlags()==1)
						BasicUnsignedMultOp(PiNum);
					else if(GetFlags()==2)
						BasicUnsignedMultOp(ENum);
            		DecimalHalf.Flags = 3;
            		break;
            	default: break;
            }
        }
    }
}