#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

inline void MediumDecV2::DivOpSameRep_CatchAll(const MediumDecV2& rValue, const RepType& LRep)
{/*
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(LRep);
	lValue.BasicUnsignedDivOp(RValue);*/
}

inline void MediumDecV2::DivOpSameRep_CatchAllV2(const MediumDecV2& rValue, const RepType& LRep)
{/*
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(LRep.ConvertFromNormalRep(rValue.GetFlags()));
	lValue.BasicUnsignedDivOp(RValue);*/
}

inline void MediumDecV2::DivOpSameRep_ApproachingBottom(const MediumDecV2& rValue)
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

inline void MediumDecV2::DivOpSameRep_ApproachingTop(const MediumDecV2& rValue)
{
	DivOpSameRep_CatchAll(rValue, RepType::ApproachingTop);
}

void MediumDecV2::UnsignedDivOp(const MediumDecV2& rValue)
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
		if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
		{
			if(DecimalHalf.Flags!=0)
				DecimalHalf.Flags = 0;
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
    #if defined(MediumDecV2_EnableUndefinedButInRange)
                case RepType::UndefinedButInRange:
                    throw "Operation not supported at moment."; break;
                    break;
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
        }
        else
        {
			if(LRep==RRep)
			{
				switch(LRep)
				{
                case RepTypeEnum::NormalType:
					BasicUnsignedDivOp(rValue);
					break;
    #if defined(AltNum_EnableApproaching)
                case RepTypeEnum::ApproachingBottom:
					DivOpSameRep_ApproachingBottom(*this, rValue);
                    break;
                case RepTypeEnum::ApproachingTop:
					DivOpSameRep_ApproachingTop(*this, rValue);
	                break;
    #endif
                default:
                    throw "Operation not supported";
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