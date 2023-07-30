#include "AltDec.hpp"

/// <summary>
/// Division Operation
/// </summary>
/// <param name="self">The self.</param>
/// <param name="Value">The value.</param>
/// <returns>MediumDecVariant&</returns>
static MediumDecVariant& MediumDecVariant::DivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
//Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
	if (self.IsZero())
		return self;
#if defined(AltNum_EnableUndefinedButinMinMaxRange)
	if (Value.ExtraRep!=UndefinedInMinMaxRangeRep&&Value.IntValue<0)
#else
	if (Value.IntValue<0)
#endif
	{
		Value.SwapNegativeStatus();
		self.SwapNegativeStatus();
	}
	#if defined(AltNum_EnableInfinityRep)
	if (Value.DecimalHalf == InfinityRep)
	{
        if(self.DecimalHalf== InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
            self.DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
        else
#if defined(AltNum_EnableApproachingValues)
        {
            self.DecimalHalf = ApproachingBottomRep;
            if(self.IntValue<0)
                self.IntValue = NegativeRep;
            else
                self.IntValue = 0;
            self.ExtraRep = 0;
        }
#else            
		    self.SetAsZero();
#endif
        return self;
	}
	if (Value.IsZero())
	{
		self.IntValue < 0 ? self.SetAsNegativeInfinity() : self.SetAsInfinity(); return self;
	}
	#else
	if (Value.IsZero())
		throw "Target value can not be divided by zero unless infinity enabled";
	#endif
	RepType LRep = self.GetRepType();
	RepType RRep = Value.GetRepType();
	if(LRep==RRep)
	{
		switch (LRep)
		{
			case RepType::NormalType:
				self.PartialDivOp(Value);
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
				self.PartialDivOp(Value);
				break;
    #endif
				
    #if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
                self.PartialDivOp(Value);
                if(Value.ExtraRep<self.ExtraRep)
                {
                    MediumDecVariant PiPowerDivisor = PiPowerNum(self.ExtraRep - Value.ExtraRep);
                    self.ExtraRep = 0;
                    self.PartialDivOp(PiPowerDivisor);
                }
                else
                {
                    self.ExtraRep -= Value.ExtraRep;
                }
                break;
    #endif

    #if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				if(self.IntValue==Value.IntValue)
				{ self.IntValue = 1; self.DecimalHalf = 0; }
				else if(self.IntValue==-Value.IntValue)
				{ self.IntValue = -1; self.DecimalHalf = 0; }
				else if(self.IntValue==NegativeRep&&Value.IntValue==0)
				{ self.IntValue = -1; self.DecimalHalf = 0; }
				else
				{
					self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
				}
				break;
			case RepType::ApproachingTop:
				if(self.IntValue==Value.IntValue)
				{ self.IntValue = 1; self.DecimalHalf = 0; self.ExtraRep = 0; }
				else if(self.IntValue==-Value.IntValue)
				{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
				else if(self.IntValue==NegativeRep&&Value.IntValue==0)
				{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
				else
				{
					self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
				}
				break;

        #if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottomDiv:
				if(self.ExtraRep==Value.ExtraRep)
				{// 0.249..9 / 0.249..9 = 1
					if(self.IntValue==Value.IntValue)
					{ self.IntValue = 1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else if(self.IntValue==-Value.IntValue)
					{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else if(self.IntValue==NegativeRep&&Value.IntValue==0)
					{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else
					{
						self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
					}
				}
				else
				{
					self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
				}
				break;
			case RepType::ApproachingTopDiv:
				if(self.ExtraRep==Value.ExtraRep)
				{//0.50..1 / 0.50..1 = 1
					if(self.IntValue==Value.IntValue)
					{ self.IntValue = 1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else if(self.IntValue==-Value.IntValue)
					{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else if(self.IntValue==NegativeRep&&Value.IntValue==0)
					{ self.IntValue = -1; self.DecimalHalf = 0; self.ExtraRep = 0; }
					else
					{
						self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
					}
				}
				else
				{
					self.CatchAllDivision(Value, LRep);//Just convert into normal numbers for now
				}
				break;
        #endif
    #endif

    #if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
			case RepType::NumByDiv://(IntValue.DecimalHalf)/ExtraRep
			//(self.(IntValue.DecimalHalf)/self.ExtraRep) / (Value.(IntValue.DecimalHalf)/Value.ExtraRep) = 
			//(self.(IntValue.DecimalHalf)* Value.ExtraRep/self.ExtraRep) /(Value.(IntValue.DecimalHalf))
				signed int DivRes = Value.ExtraRep / self.ExtraRep;
				signed int RemRes = Value.ExtraRep - self.ExtraRep / Value.ExtraRep;
				if(RemRes==0)
				{
                    if(Value.DecimalHalf==0)
                        self.ExtraRep = DivRes * Value.IntValue;
                    else
                    {
                        self.ExtraRep = DivRes;
                        self.PartialDivOp(Value);
                    }
				}
				else
				{
                    self.BasicMultOp(Value.ExtraRep);
                    self.PartialDivOp(Value);
				}
				break;
			//(Self.IntValue/self.DecimalHalf)/(Value.IntValue/Value.DecimalHalf) =
			//(self.IntValue*Value.DecimalHalf)/(self.DecimalHalf*Value.IntValue)
        #if defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
        #if defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
        #if defined(AltNum_EnableIFractional)
			case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableENum)||defined(AltNum_EnableENum)
				int NumRes = self.IntValue*Value.DecimalHalf;
                int DenomRes = self.DecimalHalf*Value.IntValue;
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
                self.DecimalHalf = 0;
                if(RemRes==0)
                {
                    self.IntValue = DivRes;
                    self.ExtraRep = 0;
                }
                else
                {
                    self.IntValue = NumRes;
                    self.ExtraRep = DenomRes;
                }
				break;
        #endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#endif
				int LeftDiv = -self.ExtraRep; int RightDiv = -Value.ExtraRep;
				AltNum NumRes = SetValue(self.IntValue, self.DecimalHalf);
				signed int DivRes = RightDiv / LeftDiv;
				signed int RemRes = RightDiv - LeftDiv / RightDiv;
				NumRes /= SetValue(Value.IntValue, Value.DecimalHalf);
				if(RemRes==0)
				{
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.ExtraRep = NumRes.ExtraRep;
				}
				else
				{
					NumRes *= RightDiv;
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.PartialDivOp(Value);
				}
				break;
		#endif
    #endif
	
    //Turn MixedFrac into fractional and then apply			
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
				self.CatchAllDivision(Value, LRep, RRep);//Temporary until more specific code
                //throw "Code not implimented yet";
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
		#endif
		#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedIFractional)
                self.ConvertAltFracWithInvertedExtra(RepType::MixedFrac); Value.ConvertAltFracWithInvertedExtra(RepType::MixedFrac);
                self.BasicMultOp(Value);
                break;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                if(Value.DecimalHalf==InfinityRep)
                {
                    if(self.DecimalHalf==InfinityRep)
                        self.SetVal(One);
                    else
#if defined(AltNum_EnableNaN)
                        self.DecimalHalf = UndefinedRep;
#else
                        throw "Undefined result(results in undefined expression)";
#endif
                }
                else
                    self.BasicDivOp(Value);
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
				throw static_cast<RepType>(LRep)-" RepType division not supported yet";
				break;
		}
	}
	else
		RepToRepDivOp(LRep, RRep, self, Value);
	return self;
}