#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

/// <summary>
/// Multiplication Operation
/// </summary>
/// <param name="Value">The rightside value.</param>
/// <returns>AltDec&</returns>
AltDec& AltDec::MultOp(AltDec& Value)
{
//Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
	if (Value.IsZero()) { SetAsZero(); return *this; }
	if (IsZero() || Value == AltDec::One)
		return *this;
#if defined(AltNum_EnableUndefinedButinMinMaxRange)
	if (Value.ExtraRep!=UndefinedInMinMaxRangeRep&&Value.IntValue<0)
#else
	if (Value.IntValue<0)
#endif
	{
		Value.SwapNegativeStatus();
		SwapNegativeStatus();
	}
	#if defined(AltNum_EnableInfinityRep)
	if (DecimalHalf == InfinityRep)
	{
		if (Value.DecimalHalf == InfinityRep && IntValue == Value.IntValue && IntValue == -1)
			IntValue = 1;
		return *this;
	}
	else if (Value.DecimalHalf == InfinityRep)
		return *this;
	#endif
	RepType LRep = GetRepType();
	RepType RRep = Value.GetRepType();
	if(LRep==RRep)
	{
		switch (LRep)
		{
			case RepType::NormalType:
				BasicMultOp(Value);
				break;
	#if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
				BasicMultOp(Value);
        #if defined(AltNum_EnablePiPowers)
                ExtraRep = -2;
        #else
				BasicMultOp(PiNum);
        #endif
				break;		
	#endif
	#if defined(AltNum_EnableENum)
			case RepType::ENum:
				BasicMultOp(Value);
				BasicMultOp(ENum);
				break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum://Xi * Yi = -XY
				ExtraRep = 0;
				BasicMultOp(-Value);
				break;
	#endif
				
	#if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
				ExtraRep += Value.ExtraRep;
                BasicMultOp(Value);
				break;
	#endif

	#if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				if (IntValue == NegativeRep)
				{
//                     if (Value.IntValue == 0)//-0.0..1 * 0.0..1
//                     {/*Do Nothing*/}
//                     else//-0.0..1 * 5.0..1
					//{/*Do Nothing*/}
					return *this;
				}
				else if (IntValue == 0)
				{
//                     if (Value.IntValue == 0)//0.0..1 * 0.0..1
//                     {/*Do Nothing*/}
//                     else//0.0..1 * 5.0..1
					//{/*Do Nothing*/}
					return *this;
				}
				else if (IntValue < 0)
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
				CatchAllMultiplicationV2(Value, LRep);
				break;

		#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingBottomDiv:
				if(Value.IntValue==0)
				{
					//-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
					//0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:2)
					// =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
					// 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:16)
					if(IntValue==0||IntValue==NegativeRep)
						ExtraRep *= Value.ExtraRep;
					else
					{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
						bool IsNegative = IntValue<0;
						if(IsNegative)
							IntValue *= -1;
						AltDec XV = SetAsApproachingMid(0, Value.ExtraRep)*IntValue;
						AltDec YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
						XV *= YV;
						if(IsNegative)
							IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
						DecimalHalf = XV.DecimalHalf;
						ExtraRep = XV.ExtraRep;
					}
					return *this;
				}
				else
				{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
					bool IsNegative = IntValue<0;
					if(IsNegative)
						IntValue = IntValue==NegativeRep:0?-IntValue;
					int XZ = IntValue * Value.IntValue;
					AltDec XV = SetAsApproachingMid(0, Value.ExtraRep)*IntValue;
					AltDec YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
					AltDec YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
					XV += XZ;
					XV += YZ+YV;
					if(IsNegative)
						IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
					DecimalHalf = XV.DecimalHalf;
					ExtraRep = XV.ExtraRep;
				}
				break;
			case RepType::ApproachingTopDiv:
				if(Value.IntValue==0)
				{
					//0.50..1 * 0.50..1(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-2)
					// =  ~0.250..1 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-4)
					if(IntValue==0||IntValue==NegativeRep)
						ExtraRep *= -Value.ExtraRep;
					else
					{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
						bool IsNegative = IntValue<0;
						if(IsNegative)
							IntValue *= -1;
						AltDec XV = SetAsApproachingMid(0, Value.ExtraRep)*IntValue;
						AltDec YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
						XV *= YV;
						if(IsNegative)
							IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
						DecimalHalf = XV.DecimalHalf;
						ExtraRep = XV.ExtraRep;
					}
					return *this;
				}
				else
				{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
					bool IsNegative = IntValue<0;
					if(IsNegative)
						IntValue = IntValue==NegativeRep:0?-IntValue;
					int XZ = IntValue * Value.IntValue;
					AltDec XV = SetAsApproachingMid(0, Value.ExtraRep)*IntValue;
					AltDec YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
					AltDec YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
					XV += XZ;
					XV += YZ+YV;
					if(IsNegative)
						IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
					DecimalHalf = XV.DecimalHalf;
					ExtraRep = XV.ExtraRep;
				}
				break;
		#endif
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
			case RepType::NumByDiv://(IntValue.DecimalHalf)/ExtraRep
			//((IntValue.DecimalHalf)/ExtraRep) * (Value.(IntValue.DecimalHalf)/Value.ExtraRep) = 
			//((IntValue.DecimalHalf)*Value.(IntValue.DecimalHalf))/(ExtraRep*Value.ExtraRep)
				BasicMultOp(Value);
				ExtraRep *= Value.ExtraRep;
				break;
				
			//(Self.IntValue*Value.IntValue)*Pi^2/(DecimalHalf/Value.DecimalHalf)
		#if defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                int NumRes = IntValue*Value.IntValue;
                bool valIsPositive = true;
                if(IntValue<0)
                    valIsPositive=false;
				int DenomRes = DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
			#ifdef AltNum_EnablePiPowers
					//Set as DivRes*Pi^2
                    IntValue = DivRes;
                    ExtraRep = -2;
			#else
					Self.SetPiVal(DivRes);
                    *this *= PiNum;
			#endif
				}
				else
				{
			#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
			#else
                IntValue = valIsPositive==false&&NumRes==0?NegativeRep:NumRes;
                DecimalHalf = 0;
                //Converting to PiNum
	            //DecimalHalf = DenomRes;
                //*this *= PiNum;
                #ifdef AltNum_EnablePiPowers
                    ExtraRep = -2;
                    PartialDivOp(DenomRes);
                #else
                    ExtraRep = PiRep;
                    BasicMultOp(PiNum);
                    PartialDivOp(DenomRes);
                #endif
			#endif
				}
				break;
		#endif
		#if defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                int NumRes = IntValue*Value.IntValue;
                bool valIsPositive = true;
                if(IntValue<0)
                    valIsPositive=false;
				int DenomRes = DecimalHalf/Value.DecimalHalf;
				//Reduce size of fractional if viable
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
				if(RemRes==0)
				{
					Self.SetEVal(DivRes);
                    *this *= ENum;
				}
				else
				{
			#ifdef AltNum_EnableBoostFractionalReduction
					//Add code here to reduce size of fractional using boost library code
			#else
                IntValue = valIsPositive==false&&NumRes==0?NegativeRep:NumRes;
                DecimalHalf = 0;
                //Converting to ENum
                ExtraRep = ERep;
                BasicMultOp(ENum);
                PartialDivOp(DenomRes);
			#endif
				}
				break;
		#endif

		#if defined(AltNum_EnableImaginaryNum)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
				//(IntValue/DecimalHalf)i*(Value.IntValue/Value.DecimalHalf)i
                //==-1*(IntValue/DecimalHalf)*(Value.IntValue/Value.DecimalHalf)
				int NumRes = -IntValue*Value.IntValue;
                int DenomRes = DecimalHalf*Value.DecimalHalf;
				signed int DivRes = NumRes / DenomRes;
				signed int RemRes = NumRes - DenomRes * NumRes;
                DecimalHalf = 0;
                if(RemRes==0)
                {
                    IntValue = DivRes;
                    ExtraRep = 0;
                }
                else
                {
                    IntValue = NumRes;
                    ExtraRep = DenomRes;
                }
				break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/-ExtraRep)*Pi Representation
			//(Value/(-ExtraRep))*Pi * (Value.Value/(-Value.ExtraRep))*Pi
			#elif defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/-ExtraRep)*e Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://(Value/-ExtraRep)*i Representation
			//(Value/(-ExtraRep))*i * (Value.Value/(-Value.ExtraRep))*i
			#endif
				ExtraRep *= -Value.ExtraRep;
				BasicMultOp(Value);
				#if defined(AltNum_EnableDecimaledPiFractionals)
					#ifdef AltNum_EnablePiPowers//Convert to PiPower representation
				PartialDivOp(-ExtraRep);
				ExtraRep = -2;
					#else
				BasicMultOp(PiNum);
					#endif
				#elif defined(AltNum_EnableDecimaledEFractionals)
				BasicMultOp(ENum);
				#else
				SwapNegativeStatus();
				#endif
				break;
		#endif
	#endif

    //Turn MixedFrac into fractional and then apply
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
				//IntValue +- (-DecimalHalf/ExtraRep) = 
				int LeftSideNum;
				if(IntValue==NegativeRep)
					LeftSideNum = DecimalHalf;
				else if(IntValue<0)
					LeftSideNum = IntValue*ExtraRep + DecimalHalf;
				else if(IntValue==0)
					LeftSideNum = -DecimalHalf;
				else
					LeftSideNum = IntValue*ExtraRep - DecimalHalf;
				int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
				//Becomes NumByDiv now
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = ExtraRep *-Value.ExtraRep;
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
				int LeftSideNum;
				if(IntValue==NegativeRep)
					LeftSideNum = DecimalHalf;
				else if(IntValue<0)
					LeftSideNum = IntValue*-ExtraRep + DecimalHalf;
				else if(IntValue==0)
					LeftSideNum = -DecimalHalf;
				else
					LeftSideNum = IntValue*-ExtraRep + -DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;

			#if defined(AltNum_EnableDecimaledEFractionals)
				//Becomes PiNumByDiv
				//And then multiply by Pi
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = ExtraRep * Value.ExtraRep;
			#elif defined(AltNum_EnablePiPowers)
				//Or convert to PiPower (of 2)
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = -2;
				PartialDivOp(ExtraRep *-Value.ExtraRep);
			#else
				//Or convert PiNum and multiply by Pi
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = -2;
				BasicMultOp(PiNum);
				PartialDivOp(ExtraRep *-Value.ExtraRep);
			#endif
				break;
				
		#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE:
				int LeftSideNum;
				if(IntValue==NegativeRep)
					LeftSideNum = DecimalHalf;
				else if(IntValue<0)
					LeftSideNum = IntValue*-ExtraRep + DecimalHalf;
				else if(IntValue==0)
					LeftSideNum = -DecimalHalf;
				else
					LeftSideNum = IntValue*-ExtraRep + -DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
			#if defined(AltNum_EnableDecimaledEFractionals)
				//Becomes ENumByDiv
				//And then multiply by e
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = ExtraRep *Value.ExtraRep;
			#else
				//Or convert ENum and multiply by e
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = -2;
				BasicMultOp(PiNum);
				PartialDivOp(ExtraRep *-Value.ExtraRep);
			#endif
				break;
		#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI:
				int LeftSideNum;
				if(IntValue==NegativeRep)
					LeftSideNum = -DecimalHalf;
				else if(IntValue<0)
					LeftSideNum = IntValue*ExtraRep - DecimalHalf;
				else if(IntValue==0)
					LeftSideNum = DecimalHalf;
				else
					LeftSideNum = IntValue*ExtraRep + DecimalHalf;
				break;
				int RightSideNum = Value.IntValue==0?-DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
				//Becomes NumByDiv now
				IntValue = LeftSideNum*RightSideNum;
				DecimalHalf = 0;
				ExtraRep = ExtraRep *-Value.ExtraRep;
				break;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                if(Value.DecimalHalf==InfinityRep)
                    DecimalHalf = InfinityRep;
                else
                    BasicMultOp(Value);
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
				throw AltDec::RepTypeAsString(LRep)-" RepType multiplication not supported yet";
				break;
		}
	}
	else
		RepToRepMultOp(LRep, RRep, *this, Value);
	return *this;
}