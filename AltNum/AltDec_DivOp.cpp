#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

/// <summary>
/// Division Operation
/// </summary>
/// <param name="self">The </param>
/// <param name="Value">The value.</param>
/// <returns>AltDec&</returns>
AltDec& AltDec::DivOp(AltDec& Value)
{
//Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
	if (IsZero())
		return *this;
#if defined(AltNum_EnableUndefinedButinMinMaxRange)
	if (Value.ExtraRep!=UndefinedInMinMaxRangeRep&&Value.IntValue<0)
#else
	if (Value.IntValue<0)
#endif
	{
		Value.SwapNegativeStatus();
		SwapNegativeStatus();
	}
	#if defined(AltNum_EnableInfinityRep)
	if (Value.DecimalHalf == InfinityRep)
	{
        if(DecimalHalf== InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
		#if defined(AltNum_EnableNaN)
            DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
		#else
			throw "Result is undefined(Infinity by Infinity)";
		#endif
        else
#if defined(AltNum_EnableApproachingValues)
        {
            DecimalHalf = ApproachingBottomRep;
            if(IntValue<0)
                IntValue = NegativeRep;
            else
                IntValue = 0;
            ExtraRep = 0;
        }
#else            
		    SetAsZero();
#endif
        return *this;
	}
	if (Value.IsZero())
	{
		IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return *this;
	}
	#else
	if (Value.IsZero())
		throw "Target value can not be divided by zero unless infinity enabled";
	#endif
	RepType LRep = GetRepType();
	RepType RRep = Value.GetRepType();
	if(LRep==RRep)
	{
		switch (LRep)
		{
			case RepType::NormalType:
				PartialDivOp(Value);
				break;
    #if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
    #endif
    #if defined(AltNum_EnableENum)
			case RepType::ENum:
    #endif
    #if defined(AltNum_EnableImaginaryNum)
			case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
    #endif
    #if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableENum) || defined(AltNum_EnableImaginaryNum)
				ExtraRep = 0;
				PartialDivOp(Value);
				break;
    #endif
				
    #if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
                PartialDivOp(Value);
                if(Value.ExtraRep<ExtraRep)
                {
                    AltDec PiPowerDivisor = PiPowerNum(ExtraRep - Value.ExtraRep);
                    ExtraRep = 0;
                    PartialDivOp(PiPowerDivisor);
                }
                else
                {
                    ExtraRep -= Value.ExtraRep;
                }
                break;
    #endif

    #if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				if(IntValue==Value.IntValue)
				{ IntValue = 1; DecimalHalf = 0; }
				else if(IntValue==-Value.IntValue)
				{ IntValue = -1; DecimalHalf = 0; }
				else if(IntValue==NegativeRep&&Value.IntValue==0)
				{ IntValue = -1; DecimalHalf = 0; }
				else
				{
					CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
				}
				break;
			case RepType::ApproachingTop:
				if(IntValue==Value.IntValue)
				{ IntValue = 1; DecimalHalf = 0; ExtraRep = 0; }
				else if(IntValue==-Value.IntValue)
				{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
				else if(IntValue==NegativeRep&&Value.IntValue==0)
				{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
				else
				{
					CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
				}
				break;

        #if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottomDiv:
				if(ExtraRep==Value.ExtraRep)
				{// 0.249..9 / 0.249..9 = 1
					if(IntValue==Value.IntValue)
					{ IntValue = 1; DecimalHalf = 0; ExtraRep = 0; }
					else if(IntValue==-Value.IntValue)
					{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
					else if(IntValue==NegativeRep&&Value.IntValue==0)
					{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
					else
					{
						CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
					}
				}
				else
				{
					CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
				}
				break;
			case RepType::ApproachingTopDiv:
				if(ExtraRep==Value.ExtraRep)
				{//0.50..1 / 0.50..1 = 1
					if(IntValue==Value.IntValue)
					{ IntValue = 1; DecimalHalf = 0; ExtraRep = 0; }
					else if(IntValue==-Value.IntValue)
					{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
					else if(IntValue==NegativeRep&&Value.IntValue==0)
					{ IntValue = -1; DecimalHalf = 0; ExtraRep = 0; }
					else
					{
						CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
					}
				}
				else
				{
					CatchAllDivisionV2(Value, LRep);//Just convert into normal numbers for now
				}
				break;
        #endif
    #endif

    #if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
			case RepType::NumByDiv://(IntValue.DecimalHalf)/ExtraRep
			{
				//((IntValue.DecimalHalf)/ExtraRep) / (Value.(IntValue.DecimalHalf)/Value.ExtraRep) = 
				//((IntValue.DecimalHalf)* Value.ExtraRep/ExtraRep) /(Value.(IntValue.DecimalHalf))
				signed int DivRes = Value.ExtraRep / ExtraRep;
				signed int RemRes = Value.ExtraRep - ExtraRep / Value.ExtraRep;
				if (RemRes == 0)
				{
					if (Value.DecimalHalf == 0)
						ExtraRep = DivRes * Value.IntValue;
					else
					{
						ExtraRep = DivRes;
						PartialDivOp(Value);
					}
				}
				else
				{
					BasicMultOp(Value.ExtraRep);
					PartialDivOp(Value);
				}
			}
			break;
			//(Self.IntValue/DecimalHalf)/(Value.IntValue/Value.DecimalHalf) =
			//(IntValue*Value.DecimalHalf)/(DecimalHalf*Value.IntValue)
        #if defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
        #if defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
        #if defined(AltNum_EnableIFractional)
			case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnablePiFractional)||defined(AltNum_EnableEFractional)||defined(AltNum_EnableIFractional)
			{
				int NumRes = Value.DecimalHalf * IntValue;
				int DenomRes = DecimalHalf * Value.IntValue;
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				DecimalHalf = 0;
				if (RemRes == 0)
				{
					IntValue = DivRes;
					ExtraRep = 0;
				}
				else
				{
					IntValue = NumRes;
					ExtraRep = DenomRes;
				}
			}
			break;
        #endif
		#if defined(AltNum_EnableDecimaledAlternativeFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#endif
			{
				int LeftDiv = -ExtraRep; int RightDiv = -Value.ExtraRep;
				AltDec NumRes = AltDec(IntValue, DecimalHalf);
				signed int DivRes = RightDiv / LeftDiv;
				signed int RemRes = RightDiv - LeftDiv / RightDiv;
				//NumRes /= AltDec(Value.IntValue, Value.DecimalHalf);
				NumRes = UnsignedBasicDiv(NumRes, AltDec(Value.IntValue, Value.DecimalHalf));
				if (RemRes == 0)
				{
					IntValue = NumRes.IntValue;
					DecimalHalf = NumRes.DecimalHalf;
					ExtraRep = NumRes.ExtraRep;
				}
				else
				{
					NumRes *= RightDiv;
					IntValue = NumRes.IntValue;
					DecimalHalf = NumRes.DecimalHalf;
					PartialDivOp(Value);
				}
			}
			break;
		#endif
    #endif
	
    //Turn MixedFrac into fractional and then apply			
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			{
				//=LeftSideNum*Value.ExtraRep / RightSideNum;
				AltDec LeftSideNum;
				if (IntValue == NegativeRep)
					LeftSideNum = AltDec(DecimalHalf);
				else if (IntValue < 0)
					LeftSideNum = AltDec(IntValue * ExtraRep + DecimalHalf);
				else if (IntValue == 0)
					LeftSideNum = AltDec(-DecimalHalf);
				else
					LeftSideNum = AltDec(IntValue * ExtraRep - DecimalHalf);
				LeftSideNum.PartialMultOp(Value.ExtraRep);
				if (LeftSideNum.IsZero())
					SetAsZero();
				else
				{
					IntValue = LeftSideNum.IntValue;
					DecimalHalf = LeftSideNum.DecimalHalf;
					ExtraRep *= Value.IntValue == 0 ? -Value.DecimalHalf : Value.IntValue * Value.ExtraRep - Value.DecimalHalf;
				}
			}
            break;//Result as NumByDiv
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
        #elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
        #endif
		#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
			{
				//=LeftSideNum*-Value.ExtraRep / RightSideNum;
				int LeftSideNum;
				if (IntValue == NegativeRep)
					LeftSideNum = DecimalHalf;
				else if (IntValue < 0)
					LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
				else if (IntValue == 0)
					LeftSideNum = -DecimalHalf;
				else
					LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
				LeftSideNum.PartialMultOp(-Value.ExtraRep);
				if (LeftSideNum.IsZero())
					SetAsZero();
				else//Result as NumByDiv
				{   //Need to have positive ExtraRep value
					//int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					IntValue = LeftSideNum.IntValue;
					DecimalHalf = LeftSideNum.DecimalHalf;
					ExtraRep *= Value.IntValue == 0 ? DecimalHalf : (Value.IntValue * Value.ExtraRep) + Value.DecimalHalf;
				}
			}
            break;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                if(Value.DecimalHalf==InfinityRep)
                {
                    if(DecimalHalf==InfinityRep)
                        SetVal(One);
                    else
#if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
#else
                        throw "Undefined result(results in undefined expression)";
#endif
                }
                else
                    BasicDivOp(Value);
                break;
        #if defined(AltNum_EnableWithinMinMaxRange)
			case RepType::WithinMinMaxRange:
                throw "Uncertain how to perform operation with unbalanced ranged";
                break;
        #endif
	#endif
    #if defined(AltNum_EnableNaN)
			case RepType::Undefined:
			case RepType::NaN:
				throw "Can't perform operations with NaN or Undefined number";
				break;
    #endif
			default:
				throw AltDec::RepTypeAsString(LRep)-" RepType division not supported yet";
				break;
		}
	}
	else
		RepToRepDivOp(LRep, RRep, *this, Value);
	return *this;
}