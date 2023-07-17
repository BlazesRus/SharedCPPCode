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
	#if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
				self.PartialMultOp(Value);
        #if defined(AltNum_EnablePiPowers)
                self.ExtraRep = -2;
        #else
				self.PartialMultOp(PiNum);
        #endif
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
				
	#if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
				self.ExtraRep += Value.ExtraRep;
                self.PartialMultOp(Value);
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
				
			//(Self.IntValue*Value.IntValue)*Pi^2/(self.DecimalHalf/Value.DecimalHalf)
		#if defined(AltNum_EnablePiRep)
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
                #if defined(AltNum_EnableDecimaledPiFractionals)
                    self.ExtraRep = -DenomRes;
                    self.PartialMultOp(PiNum);
                #else//Converting to PiNum
					//self.DecimalHalf = DenomRes;
                    //self *= PiNum;
                    #ifdef AltNum_EnablePiPowers
                        self.ExtraRep = -2;
                        self.PartialDivOp(DenomRes);
                    #else
                        self.ExtraRep = PiRep;
                        self.PartialMultOp(PiNum);
                        self.PartialDivOp(DenomRes);
                    #endif
                #endif
			#endif
				}
				break;
		#endif
		#if defined(AltNum_EnableENum)
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
                #if defined(AltNum_EnableDecimaledEFractionals)
                    self.ExtraRep = -DenomRes;
                    self.PartialMultOp(ENum);
                #else//Converting to ENum
                    self.ExtraRep = ERep;
                    self.PartialMultOp(ENum);
                    self.PartialDivOp(DenomRes);
                #endif
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
				self.PartialMultOp(Value);
				#if defined(AltNum_EnableDecimaledPiFractionals)
					#ifdef AltNum_EnablePiPowers//Convert to PiPower representation
				self.PartialDivOp(-self.ExtraRep);
				self.ExtraRep = -2;
					#else
				self.PartialMultOp(PiNum);
					#endif
				#elif defined(AltNum_EnableDecimaledEFractionals)
				self.PartialMultOp(ENum);
				#else
				self.SwapNegativeStatus();
				#endif
				break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- -DecimalHalf/ExtraRep
				throw "Code not implimented yet";
				break;
		#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://IntValue +- -DecimalHalf/-ExtraRep
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