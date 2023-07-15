#include "AltDec.hpp"

/// <summary>
/// Subtraction Operation
/// </summary>
/// <param name="self">The self.</param>
/// <param name="Value">The value.</param>
/// <returns>MediumDecVariant&</returns>
static MediumDecVariant& MediumDecVariant::DivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
//Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
	#if defined(AltNum_EnableInfinityRep)
	if (Value.DecimalHalf == InfinityRep)
	{
		self.SetAsZero(); return self;
	}
	if (Value == MediumDecVariant::Zero)
	{
		self.IntValue < 0 ? self.SetAsNegativeInfinity() : self.SetAsInfinity(); return self;
	}
	#else
	if (Value == MediumDecVariant::Zero)
		throw "Target value can not be divided by zero unless infinity enabled";
	#endif
	if (self == MediumDecVariant::Zero)
		return self;
	if (Value.IntValue < 0)
	{
		if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
		else { Value.IntValue *= -1; }
		self.SwapNegativeStatus();
	}
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
				
#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
#if defined(AltNum_EnablePiNum)
			case RepType::MixedPi:
#endif
#if defined(AltNum_EnableENum)
			case RepType::MixedE:
#endif
#if defined(AltNum_EnableImaginaryNum)
			case RepType::MixedI:
#endif
				throw "BasicMixedDivOp code not implimented yet";//self.BasicMixedDivOp(Value);
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
#ifdef AltNum_EnableBoostFractionalReduction//Add code here to reduce size of fractional using boost library code
#else
					Self.IntValue = NumRes;
					Self.DecimalHalf = DenomRes;
					Self.ExtraRep = 0;
#endif
				}
				break;
#endif
#if defined(AltNum_EnableENum)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
				
				int NumRes = Self.IntValue/Value.IntValue;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes / NumRes;
				if(RemRes==0)
				{
					Self.SetEVal(DivRes);
				}
				else
				{
#ifdef AltNum_EnableBoostFractionalReduction//Add code here to reduce size of fractional using boost library code
#else
					Self.IntValue = NumRes;
					Self.DecimalHalf = DenomRes;
#endif
				}
				Self /= ENumValue();
				break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
#endif
				break;
				
#if defined(AltNum_EnableMixedFractional)
//                    case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnablePiNum)
			case RepType::MixedPi:
#endif
#if defined(AltNum_EnableENum)
			case RepType::MixedE:
#endif
#endif
				throw "BasicMixedDivOp code not implimented yet";
				break;//
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation

				break;
				
			case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation

				break;
			case RepType::MixedI:
				throw "BasicMixedDivOp code not implimented yet";
				break;//
#endif

//#if defined(AltNum_EnableComplexNumbers)
//                    //based on https://www.varsitytutors.com/sat_mathematics-help/working-with-imaginary-numbers
//					  //(a+b)(a-b)=(a^2)-(b^2)
//                    //(x+vi)/(y+zi) =
//                    //(x-vi)/(y-zi) =
//                    //(x+vi)/(y-zi) =
//                    //(x-vi)/(y+zi) =
//                    case RepType::ComplexIRep:
//						break;
//#endif
//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//                    case RepType::UndefinedButInRange:
//						break;
//#endif
			case RepType::Undefined:
			case RepType::NaN:
				throw "Can't perform operations with NaN or Undefined number";
				break;
#if defined(AltNum_EnableNegativeZero)
			case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
				break;
#endif
			default:
				throw static_cast<RepType>(LRep)-" RepType subtraction not supported yet";
				//throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
				break;
		}
	}
	else
		RepToRepDivOp(LRep, RRep, self, Value);
	return self;
}