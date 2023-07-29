#include "AltDec.hpp"

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
	#if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
				self.BasicMultOp(Value);
        #if defined(AltNum_EnablePiPowers)
                self.ExtraRep = -2;
        #else
				self.BasicMultOp(PiNum);
        #endif
				break;		
	#endif
	#if defined(AltNum_EnableENum)
			case RepType::ENum:
				self.BasicMultOp(Value);
				self.BasicMultOp(ENum);
				break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum://Xi * Yi = -XY
				ExtraRep = 0;
				self.BasicMultOp(-Value);
				break;
	#endif
				
	#if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
				self.ExtraRep += Value.ExtraRep;
                self.BasicMultOp(Value);
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
				self.BasicMultOp(Value);
				self.ExtraRep *= Value.ExtraRep;
				break;
				
			//(Self.IntValue*Value.IntValue)*Pi^2/(self.DecimalHalf/Value.DecimalHalf)
		#if defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                int NumRes = self.IntValue*Value.IntValue;
                bool valIsPositive = true;
                if(self.IntValue<0)
                    valIsPositive=false;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
			#ifdef AltNum_EnablePiPowers
					//Set as DivRes*Pi^2
                    self.IntValue = DivRes;
                    self.ExtraRep = -2;
			#else
					Self.SetPiVal(DivRes);
                    self *= PiNum;
			#endif
				}
				else
				{
			#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
			#else
                self.IntValue = valIsPositive==false&&NumRes==0?NegativeRep:NumRes;
                self.DecimalHalf = 0;
                //Converting to PiNum
	            //self.DecimalHalf = DenomRes;
                //self *= PiNum;
                #ifdef AltNum_EnablePiPowers
                    self.ExtraRep = -2;
                    self.PartialDivOp(DenomRes);
                #else
                    self.ExtraRep = PiRep;
                    self.BasicMultOp(PiNum);
                    self.PartialDivOp(DenomRes);
			#endif
				}
				break;
		#endif
		#if defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                int NumRes = self.IntValue*Value.IntValue;
                bool valIsPositive = true;
                if(self.IntValue<0)
                    valIsPositive=false;
				int DenomRes = self.DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
					Self.SetEVal(DivRes);
                    self *= ENum;
				}
				else
				{
			#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
			#else
                self.IntValue = valIsPositive==false&&NumRes==0?NegativeRep:NumRes;
                self.DecimalHalf = 0;
                //Converting to ENum
                self.ExtraRep = ERep;
                self.BasicMultOp(ENum);
                self.PartialDivOp(DenomRes);
			#endif
				}
				break;
		#endif

		#if defined(AltNum_EnableImaginaryNum)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
				//(self.IntValue/self.DecimalHalf)i*(Value.IntValue/Value.DecimalHalf)i
                //==-1*(self.IntValue/self.DecimalHalf)*(Value.IntValue/Value.DecimalHalf)
				int NumRes = -self.IntValue*Value.IntValue;
                int DenomRes = self.DecimalHalf*Value.DecimalHalf;
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
			case RepType::PiNumByDiv://  (Value/-ExtraRep)*Pi Representation
			//(self.Value/(-self.ExtraRep))*Pi * (Value.Value/(-Value.ExtraRep))*Pi
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/-ExtraRep)*e Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/-ExtraRep)*i Representation
			//(self.Value/(-self.ExtraRep))*i * (Value.Value/(-Value.ExtraRep))*i
			#endif
				self.ExtraRep *= -Value.ExtraRep;
				self.BasicMultOp(Value);
				#if defined(AltNum_EnableDecimaledPiFractionals)
					#ifdef AltNum_EnablePiPowers//Convert to PiPower representation
				self.PartialDivOp(-self.ExtraRep);
				self.ExtraRep = -2;
					#else
				self.BasicMultOp(PiNum);
					#endif
				#elif defined(AltNum_EnableDecimaledEFractionals)
				self.BasicMultOp(ENum);
				#else
				self.SwapNegativeStatus();
				#endif
				break;
		#endif
	#endif

    //Turn MixedFrac into fractional and then apply
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
				//self.IntValue +- (-self.DecimalHalf/self.ExtraRep) = 
				int LeftSideNum;
				if(self.IntValue==NegativeRep)
					LeftSideNum = self.DecimalHalf;
				else if(self.IntValue<0)
					LeftSideNum = self.IntValue*self.ExtraRep + self.DecimalHalf;
				else if(self.IntValue==0)
					LeftSideNum = -self.DecimalHalf;
				else
					LeftSideNum = self.IntValue*self.ExtraRep - self.DecimalHalf;
				int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
				//Becomes NumByDiv now
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = self.ExtraRep *-Value.ExtraRep;
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
				int LeftSideNum;
				if(self.IntValue==NegativeRep)
					LeftSideNum = self.DecimalHalf;
				else if(self.IntValue<0)
					LeftSideNum = self.IntValue*-self.ExtraRep + self.DecimalHalf;
				else if(self.IntValue==0)
					LeftSideNum = -self.DecimalHalf;
				else
					LeftSideNum = self.IntValue*-self.ExtraRep + -self.DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;

			#if defined(AltNum_EnableDecimaledEFractionals)
				//Becomes PiNumByDiv
				//And then multiply by Pi
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = self.ExtraRep * Value.ExtraRep;
			#elif defined(AltNum_EnablePiPowers)
				//Or convert to PiPower (of 2)
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = -2;
				self.PartialDivOp(self.ExtraRep *-Value.ExtraRep);
			#else
				//Or convert PiNum and multiply by Pi
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = -2;
				self.BasicMultOp(PiNum);
				self.PartialDivOp(self.ExtraRep *-Value.ExtraRep);
			#endif
				break;
				
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
				int LeftSideNum;
				if(self.IntValue==NegativeRep)
					LeftSideNum = self.DecimalHalf;
				else if(self.IntValue<0)
					LeftSideNum = self.IntValue*-self.ExtraRep + self.DecimalHalf;
				else if(self.IntValue==0)
					LeftSideNum = -self.DecimalHalf;
				else
					LeftSideNum = self.IntValue*-self.ExtraRep + -self.DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
			#if defined(AltNum_EnableDecimaledEFractionals)
				//Becomes ENumByDiv
				//And then multiply by e
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = self.ExtraRep *Value.ExtraRep;
			#else
				//Or convert ENum and multiply by e
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = -2;
				self.BasicMultOp(PiNum);
				self.PartialDivOp(self.ExtraRep *-Value.ExtraRep);
			#endif
				break;
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
				int LeftSideNum;
				if(self.IntValue==NegativeRep)
					LeftSideNum = -self.DecimalHalf;
				else if(self.IntValue<0)
					LeftSideNum = self.IntValue*self.ExtraRep - self.DecimalHalf;
				else if(self.IntValue==0)
					LeftSideNum = self.DecimalHalf;
				else
					LeftSideNum = self.IntValue*self.ExtraRep + self.DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
				//Becomes NumByDiv now
				self.IntValue = LeftSideNum*RightSideNum;
				self.DecimalHalf = 0;
				self.ExtraRep = self.ExtraRep *-Value.ExtraRep;
				break;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    self.BasicMultOp(Value);
                    break;
        #if defined(AltNum_EnableUndefinedButinMinMaxRange)
                case RepType::UndefinedButInRange:
                    self.IntValue *= Value.IntValue.Abs();
                    self.DecimalHalf *= Value.DecimalHalf.Abs();
                    break;
        #endif
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