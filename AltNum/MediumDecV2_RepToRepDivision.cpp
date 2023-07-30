#include "MediumDecV2.hpp"

void MediumDecVariant::RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
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
			throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
            break;
        default://No nothing for most of them
        break;
    }

    //RRep Overrides before Main RepToRep Code
    switch(RRep)
    {
    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
            //Any real number / 0.0..1 = Infinity
            if(Value.IntValue==0)
            {
				self.DecimalHalf = InfinityRep;
                if(self.IntValue<0)
                    self.IntValue = -1;
                else
                    self.IntValue = 1;
				self.ExtraRep = 0;
                return;
            }
            else
            {
                Value.DecimalHalf = 1;
                RRep = RepType::NormalType;
            }
            break;

		case RepType::ApproachingTop:
            if(LRep==RepType::NormalType&&Value.IntValue==0)//1/0.9..9 = 1.0..1
            {//(For positive left side values)Techically returns self.IntValue + 0.0..self.IntValue
				self.DecimalHalf = ApproachingBottomRep;
				self.ExtraRep = 0;
                return;
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
        
        		case RepType::Undefined:
        		case RepType::NaN:
        			throw "Can't perform operations with NaN or Undefined number";
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
                    self.BasicDivOp(Value);
                    break;
				default:
				//case RepType::PiNum:
				//case RepType::PiPower:
				//case RepType::ENum:
					Value.ConvertToNormType(RRep);
					self.BasicDivOp(Value);
					break;
				}
				break;
				
			}
			break;
		default:
			throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
			break;
	}
}