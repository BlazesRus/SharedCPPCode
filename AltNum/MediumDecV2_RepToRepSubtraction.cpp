#include "MediumDecV2.hpp"

bool MediumDecVariant::RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
	bool LeftIsNegative = self.IntValue<0;
	const bool RightIsNegative = false;//bool RightIsNegative = Value.IntValue<0;//Should always return positive value before RepToRep method used
	//LRep Overrides
	switch(LRep)
	{
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
		case RepType::UndefinedButInRange:
			throw "UndefinedButInRange operations not supported yet(from left side)";
			break;
	#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		case RepType::UnknownType:
			throw "Can't perform operations with unknown/non-enabled format number";
			break;

		default://No nothing for most of them
		break;
	}

	//RRep Overrides before Main RepToRep Code
	switch(RRep)
	{
	#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			if(LRep==RepType::ApproachingTop)
			{
				if(LeftIsNegative^RightIsNegative)
				{
					self.DecimalHalf = 0;
					int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
					if(LeftIsNegative)//-X.9..9 - Y.0..1
						--RightSide;
					else//X.9..9 
						++RightSide;
					if(self.IntValue==NegativeRep)
					   self.IntValue = 0;
					self.IntValue -= RightSide;
					return;
				}
				else
				{
					Value.DecimalHalf = 1;
					RRep = RepType::NormalType;
				}
			}
			else
			{
			   Value.DecimalHalf = 1;
			   RRep = RepType::NormalType;
			}
			break;

		case RepType::ApproachingTop:
			if(LRep==RepType::ApproachingBottom)
			{
				if(LeftIsNegative^RightIsNegative)
				{
					self.DecimalHalf = 0;
					int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				   if(LeftIsNegative)//-X.0..1 - Y.9..9 = X+Y+1
						--RightSide;
				   else//X.0..1 - -Y.9..9 = X+Y+1
						++RightSide;
					if(self.IntValue==NegativeRep)
					   self.IntValue = 0;
					self.IntValue -= RightSide;
					return;
				}
				else
				{
					Value.DecimalHalf = 999999999;
					Value.ExtraRep = 0;
					RRep = RepType::NormalType;
				}
			}
			else
			{
				Value.DecimalHalf = 999999999;
				Value.ExtraRep = 0;
				RRep = RepType::NormalType;
			}
			break;
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
		case RepType::UndefinedButInRange:
			throw "UndefinedButInRange operations not supported yet(from right side)";
			break;
	#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		case RepType::UnknownType:
			throw "Can't perform operations with unknown/non-enabled format number";
			break;
		default://No nothing for most of them
		break;
	}

	switch (LRep)//Main switch block starts here
	{
		case RepType::NormalType:
			switch (RRep)
			{
				case RepType::NormalType://Fail safe for when converted before switch
					self.BasicSubOp(Value);
					break;
				default:
//						case RepType::PiNum:
//                      case RepType::ENum:
					Value.ConvertToNormType(RRep);
					self.BasicSubOp(Value);
					break;
			}
			break;
		default:
			throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
			break;
	}
}