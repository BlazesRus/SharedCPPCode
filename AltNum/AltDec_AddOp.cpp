#include "AltDec.hpp"

/// <summary>
/// Addition Operation
/// </summary>
/// <param name="self">The self.</param>
/// <param name="Value">The value.</param>
/// <returns>AltDec</returns>
static MediumDecVariant& MediumDecVariant::AddOp(MediumDecVariant& self, MediumDecVariant& Value)
{
	if (self == Zero)
	{
		self.IntValue = Value.IntValue; self.DecimalHalf = Value.DecimalHalf;
		self.ExtraRep = Value.ExtraRep; return self;
	}
	else if (Value == Zero)
		return self;
	#if defined(AltNum_EnableInfinityRep)
	if (self.DecimalHalf==InfinityRep)
		return self;
	if (Value.DecimalHalf == InfinityRep)
	{
		Value.IntValue == 1 ? self.SetAsInfinity() : self.SetAsNegativeInfinity();
		return self;
	}
	#endif
	RepType LRep = self.GetRepType();
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
	#if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
	#endif
				self.BasicAddOp(Value);
				break;
				
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
		#endif
				throw "Code not implimented yet";//self.BasicMixedAddOp(Value);
				break;
	#endif

	#if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				if (self.IntValue == NegativeRep)
				{
					if (Value.IntValue == 0)//-0.0..1 + 0.0..1 = 0
						self.SetAsZero();
					else if (Value.IntValue == NegativeRep)//-0.0..1 - 0.0..1 = -0.0..1
					{/*Do Nothing*/}
					else if (Value.IntValue < 0)//-0.0..1 - 1.0..1 = -1.0..1
					{
						self.IntValue = Value.IntValue;
					}
					else//-0.0..1 + 5.0..1 = 5
					{
						DecimalHalf = 0;
						self.IntValue = Value.IntValue;
					}
				}
				if (self.IntValue == 0)
				{
					if (Value.IntValue == 0)//0.0..1 + 0.0..1 = 0.0..1
					{/*Do Nothing*/}
					else if (Value.IntValue == NegativeRep)//0.0..1 - 0.0..1 = 0
						self.SetAsZero();
					else if (Value.IntValue < 0)//0.0..1 - 1.0..1 = -1
					{
						DecimalHalf = 0;
						self.IntValue = Value.IntValue;
					}
					else//0.0..1 + 5.0..1 = 5.0..1
					{
						self.IntValue = Value.IntValue;
					}
				}
				else if (self.IntValue < 0)
				{
					if (Value.IntValue == 0)//-1.0..1 + 0.0..1  = -1
						DecimalHalf = 0;
					else if (Value.IntValue == NegativeRep)//-1.0..1 - 0.0..1 = -1.0..1
					{/*Do Nothing*/}
					else if(self.IntValue==-Value.IntValue)//-1.01 + 1.01
						self.SetAsZero();
					else if (Value.IntValue < 0)//-1.0..1 - 2.0..1
					{
						self.IntValue += Value.IntValue;
					}
					else//-1.0..1 + 2.0..1 = 1
					{
						DecimalHalf = 0;
						self.IntValue += Value.IntValue;
					}
				}
				else
				{
					if (Value.IntValue == 0)//1.0..1 + 0.0..1
					{/*Do Nothing*/}
					else if (Value.IntValue == NegativeRep)//1.0..1 - 0.0..1
						DecimalHalf = 0;
					else if(self.IntValue==-Value.IntValue)//1.0..1 - 1.0..1
						self.SetAsZero();
					else if (Value.IntValue < 0)// 1.0..1  - 2.0..1
					{
						DecimalHalf = 0;
						self.IntValue += Value.IntValue;
					}
					else//1.0..1 + 1.0..1
					{
						self.IntValue += Value.IntValue;
					}
				}
				break;
			case RepType::ApproachingTop:
				if (self.IntValue == NegativeRep)
				{
					if (Value.IntValue == 0)//-0.9..9 + 0.9..9 = 0
						self.SetAsZero();
					else if (Value.IntValue == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
						self.IntValue = -1;
					else if (Value.IntValue < 0)//-0.9..9 - 1.9..9 = -2.9..8
					{
						self.IntValue = Value.IntValue-1;
					}
					else//-0.9..9 + 5.9..9 = 5
					{
						DecimalHalf = 0; ExtraRep = 0;
						self.IntValue = Value.IntValue;
					}
				}
				if (self.IntValue == 0)
				{
					if (Value.IntValue == 0)//0.9..9 + 0.9..9 = 1.9..8
						self.IntValue = 1;
					else if (Value.IntValue == NegativeRep)//0.9..9 - 0.9..9 = 0
						self.SetAsZero();
					else if (Value.IntValue < 0)//0.9..9 - 1.9..9 = -1
					{
						DecimalHalf = 0; ExtraRep = 0;
						self.IntValue = Value.IntValue;
					}
					else//0.9..9 + 5.9..9 = 6.9..8
					{
						self.IntValue = Value.IntValue+1;
					}
				}
				else if (self.IntValue < 0)
				{
					if (Value.IntValue == 0)//-1.9..9 + 0.9..9  = -1
					{	DecimalHalf = 0; ExtraRep = 0; }
					else if (Value.IntValue == NegativeRep)//-1.9..9 - 0.9..9 = -2.9..9
						self.IntValue += Value.IntValue;
					else if(self.IntValue==-Value.IntValue)//-1.9..9 + 1.9..9
						self.SetAsZero();
					else if (Value.IntValue < 0)//-1.9..9 - 2.9..9
					{
						self.IntValue += Value.IntValue;
					}
					else//-1.9..9 + 2.9..9
					{
						DecimalHalf = 0; ExtraRep = 0;
						self.IntValue += Value.IntValue;
					}
				}
				else
				{
					if (Value.IntValue == 0)//1.9..9 + 0.9..9 = 2.9..8
					{
						if(self.IntValue==-1)
							self.IntValue = NegativeRep;
						else
							++self.IntValue;
					}
					else if (Value.IntValue == NegativeRep)//1.9..9 - 0.9..9
					{	DecimalHalf = 0; ExtraRep = 0; }
					else if(self.IntValue==-Value.IntValue)//1.9..9 - 1.9..9
						self.SetAsZero();
					else if (Value.IntValue < 0)// 1.9..9  - 2.9..9
					{
						DecimalHalf = 0; ExtraRep = 0;
						self.IntValue += Value.IntValue;
					}
					else//1.9..9 + 1.9..9 = 3.9..8
					{
						self.IntValue += Value.IntValue+1;
					}
				}
				break;

		#if defined(AltNum_EnableApproachingDivided)
//			case RepType::ApproachingBottomDiv:
//                        break;
//			case RepType::ApproachingTopDiv:
//                        break;
		#endif
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
			case RepType::NumByDiv:
		#if defined(AltNum_EnablePiRep)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
		#if defined(AltNum_EnableENum)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
		#if defined(AltNum_EnableINum)
			case RepType::IFractional://  IntValue/DecimalHalf*e Representation
		#endif
		#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableENum)||defined(AltNum_EnableINum)
				if (self.DecimalHalf == Value.DecimalHalf)
				{
				
					self += Value.IntValue;
				}
				else
				{
					self.ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
					self.BasicAddOp(Value);
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
				if (self.ExtraRep == Value.ExtraRep)
					self.BasicAddOp(Value);
				else
				{
					self.ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
					self.BasicAddOp(Value);
				}
				break;
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
	#if defined(AltNum_EnableNegativeZero)
			case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
				return self;
                break;
	#endif
			default:
				throw static_cast<RepType>(LRep)+" RepType addition not supported yet";
				break;
		}
	}
	else
	{
		RepToRepAddOp(LRep, RRep, self, Value);
	}
	return self;
}