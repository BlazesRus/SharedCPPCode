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
	if (Value.IntValue < 0)
	{
		if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
		else { Value.IntValue *= -1; }
		self.SwapNegativeStatus();
	}
	#if defined(AltNum_EnableInfinityRep)
	if (Value.DecimalHalf == InfinityRep)
	{
		self.SetAsZero(); return self;
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
    #if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
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
				
    #if defined(AltNum_EnablePiRep)&&defined(AltNum_EnablePiPowers)
			case RepType::PiNum:
				//Add code that converts into PiPower type representation here later
				break;
			case RepType::PiPower:
				//Add Pi powers code here later
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
			//(self.(IntValue.DecimalHalf)* Value.ExtraRep/self.ExtraRep) /(Value.IntValue.DecimalHalf)
				AltNum NumRes = SetValue(self.IntValue, self.DecimalHalf);
				signed int DivRes = Value.ExtraRep / self.ExtraRep;
				signed int RemRes = Value.ExtraRep - self.ExtraRep / Value.ExtraRep;
				NumRes /= SetValue(Value.IntValue, Value.DecimalHalf);
				if(RemRes==0)
				{
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.ExtraRep = 
				}
				else
				{
					NumRes *= Value.ExtraRep;
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.PartialDivOp(Value);
				}
				break;
			//(Self.IntValue/self.DecimalHalf)/(Value.IntValue/Value.DecimalHalf) =
			//(Self.IntValue/self.DecimalHalf)
        #if defined(AltNum_EnablePiRep)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
        #if defined(AltNum_EnableENum)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
        #if defined(AltNum_EnableImaginaryNum)
			case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableENum)||defined(AltNum_EnableENum)
				int NumRes = Self.IntValue/Value.IntValue;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes / NumRes;
				if(RemRes==0)
				{
					Self.SetVal(DivRes);
				}
				else
				{
            #ifdef AltNum_EnableBoostFractionalReduction
                //Add code here to reduce size of fractional using boost library code
            #else
					Self.IntValue = NumRes;
					Self.DecimalHalf = DenomRes;
					Self.ExtraRep = 0;
            #endif
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
				self.ExtraRep = -self.ExtraRep; Value.ExtraRep = -Value.ExtraRep;
				AltNum NumRes = SetValue(self.IntValue, self.DecimalHalf);
				signed int DivRes = Value.ExtraRep / self.ExtraRep;
				signed int RemRes = Value.ExtraRep - self.ExtraRep / Value.ExtraRep;
				NumRes /= SetValue(Value.IntValue, Value.DecimalHalf);
				if(RemRes==0)
				{
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.ExtraRep = 
				}
				else
				{
					NumRes *= Value.ExtraRep;
					self.IntValue = NumRes.IntValue;
					self.DecimalHalf = NumRes.DecimalHalf;
					self.PartialDivOp(Value);
				}
				break;
		#endif
    #endif
				
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
				throw "Code not implimented yet";
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://(IntValue +- (DecimalHalf*-1)/-ExtraRep)*Pi
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
		#endif
		#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)||defined(AltNum_EnableMixedIFractional)
				throw "Code not implimented yet";
				break;
		#endif
	#endif

	#if defined(AltNum_EnableComplexNumbers)
//                    //based on https://www.varsitytutors.com/sat_mathematics-help/working-with-imaginary-numbers
//					  //(a+b)(a-b)=(a^2)-(b^2)
//                    //(x+vi)/(y+zi) =
//                    //(x-vi)/(y-zi) =
//                    //(x+vi)/(y-zi) =
//                    //(x-vi)/(y+zi) =
//                    case RepType::ComplexIRep:
//						break;
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//                    case RepType::UndefinedButInRange:
//						break;
    #endif
			case RepType::Undefined:
			case RepType::NaN:
				throw "Can't perform operations with NaN or Undefined number";
				break;
			default:
				throw static_cast<RepType>(LRep)-" RepType division not supported yet";
				break;
		}
	}
	else
		RepToRepDivOp(LRep, RRep, self, Value);
	return self;
}