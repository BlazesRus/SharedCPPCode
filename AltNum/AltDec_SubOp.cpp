#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

/// <summary>
/// Subtraction Operation
/// </summary>
/// <param name="Value">The value.</param>
/// <returns>AltDec&</returns>
AltDec& AltDec::SubOp(AltDec& Value)
{
	if (IsZero())
	{
		IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
		ExtraRep = Value.ExtraRep; return *this;
	}
	else if (Value.IsZero())
		return *this;
	#if defined(AltNum_EnableInfinityRep)
	if (DecimalHalf == InfinityRep)
		return *this;
	if (Value.DecimalHalf == InfinityRep)
	{
		Value.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
		return *this;
	}
	#endif
	RepType LRep = GetRepType();
	RepType RRep = Value.GetRepType();
	if(LRep==RRep)
	{
		switch (LRep)
		{
			case RepType::NormalType:
	#if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
	#endif
	#if defined(AltNum_EnableENum)
			case RepType::ENum:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum:
	#endif
				BasicSubOp(Value);
				break;

	#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottom:
				if (IntValue == NegativeRep)
				{
					if (Value.IntValue == 0)//-0.0..1 - 0.0..1
					{/*Do Nothing*/}
					else if (Value.IntValue == NegativeRep)//-0.0..1 + 0.0..1 
						SetAsZero();
					else if (Value.IntValue < 0)//-0.0..1 + 1.0..1
					{
						DecimalHalf = 0;
						IntValue = -Value.IntValue;
					}
					else//-0.0..1 - 5.0..1
					{
						IntValue = -Value.IntValue;
					}
				}
				if (IntValue == 0)
				{
					if (Value.IntValue == 0)//0.0..1 - 0.0..1 = 0.0..1
					{/*Do Nothing*/}
					else if (Value.IntValue == NegativeRep)//0.0..1 + 0.0..1 = 0

					else if (Value.IntValue < 0)//0.0..1 + 1.0..1
					{
						IntValue = -Value.IntValue;
					}
					else//0.0..1 - 5.0..1
					{
						DecimalHalf = 0;
						IntValue = -Value.IntValue;
					}
				}
				else if (IntValue < 0)
				{
					if (Value.IntValue == 0)//-1.0..1 - 0.0..1  = -1
					{/*Do Nothing*/}
					else if (Value.IntValue == NegativeRep)//-1.0..1 + 0.0..1
						DecimalHalf = 0;
					else if(IntValue==Value.IntValue)//-1.01 - 1.01
						SetAsZero();
					else if (Value.IntValue < 0)//-1.0..1 + 2.0..1
					{
						DecimalHalf = 0;
						IntValue -= Value.IntValue;
					}
					else//-1.0..1 - 2.0..1 = 1
					{
						IntValue -= Value.IntValue;
					}
				}
				else
				{
					if (Value.IntValue == 0)//1.0..1 - 0.0..1
						DecimalHalf = 0;
					else if (Value.IntValue == NegativeRep)//1.0..1 + 0.0..1
					{/*Do Nothing*/}
					else if(IntValue==-Value.IntValue)//1.0..1 - 1.0..1
						SetAsZero();
					else if (Value.IntValue < 0)// 1.0..1  - 2.0..1
					{
						DecimalHalf = 0;
						IntValue -= Value.IntValue;
					}
					else//1.0..1 - 1.0..1
					{
						IntValue -= Value.IntValue;
					}
				}
				break;
			case RepType::ApproachingTop:
				if (IntValue == NegativeRep)
				{
					if (Value.IntValue == 0)//-0.9..9 - 0.9..9 = 0
						IntValue = -1;
					else if (Value.IntValue == NegativeRep)//-0.9..9 + 0.9..9
						SetAsZero();
					else if (Value.IntValue < 0)//-0.9..9 + 1.9..9

					else//-0.9..9 - 5.9..9 = -6.9..8
						IntValue = -Value.IntValue-1;
				}
				if (IntValue == 0)
				{
					if (Value.IntValue == 0)//0.9..9 - 0.9..9
						SetAsZero();
					else if (Value.IntValue == NegativeRep)//0.9..9 + 0.9..9 = 1.9..8
						IntValue = 1;
					else if (Value.IntValue < 0)//0.9..9 + 1.9..9 = 1.9..8
						IntValue = -Value.IntValue;
					else//0.9..9 - 5.9..9 = -5
					{
						DecimalHalf = 0; ExtraRep = 0;
						IntValue = -Value.IntValue;
					}
				}
				else if (IntValue < 0)
				{
					if (Value.IntValue == 0)//-1.9..9 - 0.9..9  = -2.9..8
						--IntValue;
					else if (Value.IntValue == NegativeRep)//-1.9..9  + 0.9..9 = -1
					{ DecimalHalf = 0; ExtraRep = 0; }
					else if(IntValue==Value.IntValue)//-1.01 + 1.01
						SetAsZero();
					else if (Value.IntValue < 0)//-1.9..9 + 2.9..9
					{
						DecimalHalf = 0; ExtraRep = 0;
						IntValue -= Value.IntValue;
					}
					else//-1.9..9 - 2.9..9
					{
						IntValue -= Value.IntValue+1;
					}
				}
				else
				{
					if (Value.IntValue == 0)//1.9..9 - 0.9..9 = 2.9..8
					{	DecimalHalf = 0; ExtraRep = 0;}
					else if (Value.IntValue == NegativeRep)//1.9..9 + 0.9..9
						++IntValue;
					else if(IntValue==-Value.IntValue)//1.9..9 - 1.9..9
						SetAsZero();
					else if (Value.IntValue < 0)// 1.9..9  - 2.9..9
					{
						DecimalHalf = 0;
						IntValue -= Value.IntValue;
					}
					else//1.9..9 - 2.9..9
					{
						DecimalHalf = 0; ExtraRep = 0;
						IntValue -= Value.IntValue;
					}
				}
				break;

		#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottomDiv:
                CatchAllSubtraction(Value, RepType::ApproachingBottomDiv);
                break;
			case RepType::ApproachingTopDiv:
                CatchAllSubtraction(Value, RepType::ApproachingTopDiv);    
                break;
		#endif
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
			case RepType::NumByDiv:
		#if defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
		#if defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
		#if defined(AltNum_EnableIFractional)
			case RepType::IFractional://  IntValue/DecimalHalf*e Representation
		#endif
		#if defined(AltNum_EnablePiFractional)||defined(AltNum_EnableEFractional)||defined(AltNum_EnableIFractional)
				if (DecimalHalf == Value.DecimalHalf)
				{
					*this -= Value.IntValue;
				}
				else
				{
					ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
					BasicSubOp(Value);
				}
				break;
		#endif

		#if defined(AltNum_EnableDecimaledAlternativeFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/(ExtraRep*-1))*e Representation
			#endif
				if (ExtraRep == Value.ExtraRep)
					BasicSubOp(Value);
				else
				{
					ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
					BasicSubOp(Value);
				}
				break;
		#endif
	#endif

	#if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                if(ExtraRep!=Value.ExtraRep)
                {
                    ConvertPiPowerToPiRep(); value.ConvertPiPowerToPiRep();
                }
				BasicSubOp(Value);
				break;
	#endif
			
	#if defined(AltNum_EnableMixedFractional)
            //Ignoring the fact that mixed fraction could be improperly formatted for spend
            //because should be corrected after every operation
			case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
				if(ExtraRep==Value.ExtraRep)
                {
                    IntValue -= Value.IntValue;
                    DecimalHalf -= Value.DecimalHalf;
                    if(DecimalHalf<0)//Subtract the overflow
                    {
                        DecimalHalf += ExtraRep;
                        --IntValue;
                    }
                    else//Add the overflow
                    {
                        DecimalHalf += ExtraRep;
                        ++IntValue;
                    }
                }
                else
                {
                    IntValue *= Value.ExtraRep;
                    IntValue -= Value.IntValue*ExtraRep;
                    DecimalHalf -= Value.DecimalHalf*ExtraRep;
                    if(DecimalHalf<0)//Subtract the overflow
                    {
                        DecimalHalf += ExtraRep;
                        --IntValue;
                    }
                    else//Add the overflow
                    {
                        DecimalHalf += ExtraRep;
                        ++IntValue;
                    }
                    ExtraRep *= Value.ExtraRep;
                }
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
        #elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
		#endif
				if(ExtraRep==Value.ExtraRep)
                {
                    IntValue -= Value.IntValue;
                    DecimalHalf -= Value.DecimalHalf;
                    if(DecimalHalf>0)//Subtract the overflow
                    {
                        DecimalHalf += ExtraRep;
                        --IntValue;
                    }
                    else//Add the overflow
                    {
                        DecimalHalf += ExtraRep;
                        ++IntValue;
                    }
                }
                else
                {
                    IntValue *= -Value.ExtraRep;
                    IntValue -= Value.IntValue*-ExtraRep;
                    DecimalHalf -= Value.DecimalHalf*-ExtraRep;
                    if(DecimalHalf>0)//Subtract the overflow
                    {
                        DecimalHalf += ExtraRep;
                        --IntValue;
                    }
                    else//Add the overflow
                    {
                        DecimalHalf += ExtraRep;
                        ++IntValue;
                    }
                    ExtraRep *= -Value.ExtraRep;
                }
				break;
        #endif
	#endif

	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
			case RepType::UndefinedButInRange:
                BasicSubOp(Value);
				break;
        #if defined(AltNum_EnableWithinMinMaxRange)
			case RepType::WithinMinMaxRange:
                if(IntValue==NegativeRep)
                {
                    if(Value.IntValue==NegativeRep)
                        IntValue = 0;
                }
                else if(Value.IntValue==NegativeRep)
                {//+positive infinity to left side
            #if defined(AltNum_EnableNaN)
                    DecimalHalf = UndefinedRep;
            #else
                throw "Uncertain result";
            #endif
                }
                else
                    IntValue -= Value.IntValue;
                if(DecimalHalf==InfinityRep)
                {
                    if(Value.DecimalHalf==InfinityRep)
                        DecimalHalf = 0;
                }
                else if(Value.DecimalHalf==InfinityRep)
                {// subtractive infinity from real number
            #if defined(AltNum_EnableNaN)
                    DecimalHalf = UndefinedRep;
            #else
                    throw "Uncertain result";
            #endif
                }
                else
                    DecimalHalf -= Value.DecimalHalf;
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
				throw static_cast<RepType>(LRep)-" RepType subtraction not supported yet";
				break;
		}
	}
	else
	{
		RepToRepSubOp(LRep, RRep, *this, Value);
	}
	return *this;
}