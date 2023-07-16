#include "AltDec.hpp"

/// <summary>
/// Multiplication Operation
/// </summary>
/// <param name="self">The self.</param>
/// <param name="Value">The value.</param>
/// <returns>MediumDecVariant&</returns>
static MediumDecVariant& MediumDecVariant::MultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
//Warning:Modifies Value to make it a positive variable
	if (Value.IsZero()) { self.SetAsZero(); return self; }
	if (self.IsZero() || Value == MediumDecVariant::One)
		return self;
	if (Value<0)
	{
		if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
		else { Value.IntValue *= -1; }
		self.SwapNegativeStatus();
	}
	#if defined(AltNum_EnableInfinityRep)
	if (self.DecimalHalf == InfinityRep)
	{
		if (Value.DecimalHalf == InfinityRep && self.IntValue == Value.IntValue && self.IntValue == -1)
			self.IntValue = 1;
		return self;
	}
	else if (Value.DecimalHalf == InfinityRep)
		return self;
	#endif
	RepType LRep = self.GetRepType();
	RepType RRep = Value.GetRepType();
	if(LRep==RRep)
	{
		switch (LRep)
		{
			case RepType::NormalType:
				self.PartialMultOp(Value);
				break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
			case RepType::PiNum:
				self.PartialMultOp(Value);
				self.PartialMultOp(PiNum);
				break;		
	#endif
	#if defined(AltNum_EnableENum)
			case RepType::ENum:
				self.PartialMultOp(Value);
				self.PartialMultOp(ENum);
				break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum://Xi * Yi = -XY
				ExtraRep = 0;
				self.PartialMultOp(-Value);
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
				if (self.IntValue == NegativeRep)
				{
//                     if (Value.IntValue == 0)//-0.0..1 * 0.0..1
//                     {/*Do Nothing*/}
//                     else//-0.0..1 * 5.0..1
					//{/*Do Nothing*/}
					return self;
				}
				else if (self.IntValue == 0)
				{
//                     if (Value.IntValue == 0)//0.0..1 * 0.0..1
//                     {/*Do Nothing*/}
//                     else//0.0..1 * 5.0..1
					//{/*Do Nothing*/}
					return self;
				}
				else if (self.IntValue < 0)
				{
					if (Value.IntValue == 0)//-1.0..1 * 0.0..1
					{	IntValue = NegativeRep; }
					else//-1.0..1 * 2.0..1
						IntValue *= Value.IntValue;
				}
				else
				{
					if (Value.IntValue == 0)//1.0..1 * 0.0..1
					{	IntValue = 0; }
					else//1.0..1 * 2.0..1
						IntValue *= Value.IntValue;
				}
				break;
			case RepType::ApproachingTop://Just going to convert into normal numbers for now
				self.CatchAllMultiplication(Value, LRep);
				break;

		#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottomDiv:
				if(Value.IntValue==0)
				{
					//-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
					//0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:2)
					// =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
					// 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:16)
					if(self.IntValue==0||self.IntValue==NegativeRep)
						self.ExtraRep *= Value.ExtraRep;
					else
					{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
						bool IsNegative = self.IntValue<0;
						if(IsNegative)
							self.IntValue *= -1;
						MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
						MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
						XV *= YV;
						if(IsNegative)
							self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
						self.DecimalHalf = XV.DecimalHalf;
						self.ExtraRep = XV.ExtraRep;
					}
					return self;
				}
				else
				{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
					bool IsNegative = self.IntValue<0;
					if(IsNegative)
						self.IntValue = self.IntValue==NegativeRep:0?-self.IntValue;
					int XZ = self.IntValue * Value.IntValue;
					MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
					MediumDecVariant YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
					MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
					XV += XZ;
					XV += YZ+YV;
					if(IsNegative)
						self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
					self.DecimalHalf = XV.DecimalHalf;
					self.ExtraRep = XV.ExtraRep;
				}
				break;
			case RepType::ApproachingTopDiv:
				if(Value.IntValue==0)
				{
					//0.50..1 * 0.50..1(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-2)
					// =  ~0.250..1 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-4)
					if(self.IntValue==0||self.IntValue==NegativeRep)
						self.ExtraRep *= -Value.ExtraRep;
					else
					{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
						bool IsNegative = self.IntValue<0;
						if(IsNegative)
							self.IntValue *= -1;
						MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
						MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
						XV *= YV;
						if(IsNegative)
							self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
						self.DecimalHalf = XV.DecimalHalf;
						self.ExtraRep = XV.ExtraRep;
					}
					return self;
				}
				else
				{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
					bool IsNegative = self.IntValue<0;
					if(IsNegative)
						self.IntValue = self.IntValue==NegativeRep:0?-self.IntValue;
					int XZ = self.IntValue * Value.IntValue;
					MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
					MediumDecVariant YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
					MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
					XV += XZ;
					XV += YZ+YV;
					if(IsNegative)
						self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
					self.DecimalHalf = XV.DecimalHalf;
					self.ExtraRep = XV.ExtraRep;
				}
				break;
		#endif
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
			case RepType::NumByDiv://(IntValue.DecimalHalf)/ExtraRep
			//(self.(IntValue.DecimalHalf)/self.ExtraRep) * (Value.(IntValue.DecimalHalf)/Value.ExtraRep) = 
			//(self.(IntValue.DecimalHalf)*Value.(IntValue.DecimalHalf))/(self.ExtraRep*Value.ExtraRep)
				self.PartialMultOp(Value);
				self.ExtraRep *= Value.ExtraRep;
				break;
				
			//(Self.IntValue*Value.IntValue)/(self.DecimalHalf/Value.DecimalHalf)
		#if defined(AltNum_EnablePiRep)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
				int NumRes = Self.IntValue*Value.IntValue;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
			#ifdef AltNum_EnablePiPowers
					//Set as DivRes*Pi^2
			#else
					Self.SetEVal(DivRes);
			#endif
				}
				else
				{
			#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
			#else
					Self.IntValue = NumRes;
					Self.DecimalHalf = DenomRes;
			#endif
				}
			#ifndef AltNum_EnablePiPowers
				Self *= PiNumValue();
			#endif
				break;
		#endif
		#if defined(AltNum_EnableENum)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
				int NumRes = Self.IntValue*Value.IntValue;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
					Self.SetEVal(DivRes);
				}
				else
				{
				#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
				#else
					Self.IntValue = NumRes;
					Self.DecimalHalf = DenomRes;
				#endif
				}
				Self *= ENumValue();
				break;
		#endif

		#if defined(AltNum_EnableImaginaryNum)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
				//(self.IntValue/self.DecimalHalf)i*(Value.IntValue/Value.DecimalHalf)i
                //==-1*(self.IntValue/self.DecimalHalf)*(Value.IntValue/Value.DecimalHalf)
				throw "Code not implimented yet";
				break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
				throw "Code not implimented yet";
				break;
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
				throw "Code not implimented yet";
				break;
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
				throw "Code not implimented yet";
				break;
			#endif
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
		#endif
				throw "Code not implimented yet";
				break;
	#endif

	#if defined(AltNum_EnableComplexNumbers)
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
				throw static_cast<RepType>(LRep)-" RepType multiplication not supported yet";
				break;
		}
	}
	else
		RepToRepMultOp(LRep, RRep, self, Value);
	return self;
}