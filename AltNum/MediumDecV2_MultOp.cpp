#include "MediumDecV2.hpp"

/// <summary>
/// Multiplication Operation
/// </summary>
/// <param name="self">The self.</param>
/// <param name="Value">The value.</param>
/// <returns>MediumDecVariant&</returns>
static MediumDecVariant& MediumDecVariant::MultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
//Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
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
				self.BasicMultOp(Value);
				break;
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